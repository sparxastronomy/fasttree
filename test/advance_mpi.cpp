#include "../src/hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace fasttree;

// Dynamic MPI Datatype traits
template <typename T>
struct mpi_type_traits {
  static MPI_Datatype type() {
    if constexpr (sizeof(T) == 1)
      return MPI_UINT8_T;
    else if constexpr (sizeof(T) == 2)
      return MPI_UINT16_T;
    else if constexpr (sizeof(T) == 4)
      return MPI_UINT32_T;
    else if constexpr (sizeof(T) == 8)
      return MPI_UINT64_T;
    else
      return MPI_BYTE;
  }
};

template <>
struct mpi_type_traits<float> {
  static MPI_Datatype type() { return MPI_FLOAT; }
};

template <>
struct mpi_type_traits<double> {
  static MPI_Datatype type() { return MPI_DOUBLE; }
};

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    if (rank == 0) std::cerr << "Advance MPI test requires exactly 2 ranks.\n";
    MPI_Finalize();
    return 0;
  }

  sycl::queue q;
  const int n = 100000;

  // Domain setup:
  // Rank 0 owns [0, 100] in X. Halo region is [90, 100].
  // Rank 1 owns [100, 200] in X. Halo region is [100, 110].
  double rank_min_val = rank * 100.0;
  coord_t halo_min, halo_max;
  int neighbor;

  if (rank == 0) {
#if defined(FASTTREE_INTEGER_COORDS)
    halo_min = float_to_int_rep(90.0, 0.0, 200.0);
    halo_max = float_to_int_rep(100.0, 0.0, 200.0);
#else
    halo_min = static_cast<coord_t>(90.0);
    halo_max = static_cast<coord_t>(100.0);
#endif
    neighbor = 1;
  } else {
#if defined(FASTTREE_INTEGER_COORDS)
    halo_min = float_to_int_rep(100.0, 0.0, 200.0);
    halo_max = float_to_int_rep(110.0, 0.0, 200.0);
#else
    halo_min = static_cast<coord_t>(100.0);
    halo_max = static_cast<coord_t>(110.0);
#endif
    neighbor = 0;
  }

  particles<coord_t> p;
  p.pos_x.resize(n);
  p.pos_y.resize(n);
  p.pos_z.resize(n);

  // Generate uniform particles across the domain
  for (int i = 0; i < n; ++i) {
    double px = rank_min_val + (static_cast<double>(i) / n) * 100.0;
    double py = (static_cast<double>(rand()) / RAND_MAX) * 100.0;
    double pz = (static_cast<double>(rand()) / RAND_MAX) * 100.0;
#if defined(FASTTREE_INTEGER_COORDS)
    p.pos_x[i] = float_to_int_rep(px, 0.0, 200.0);
    p.pos_y[i] = float_to_int_rep(py, 0.0, 200.0);
    p.pos_z[i] = float_to_int_rep(pz, 0.0, 200.0);
#else
    p.pos_x[i] = static_cast<coord_t>(px);
    p.pos_y[i] = static_cast<coord_t>(py);
    p.pos_z[i] = static_cast<coord_t>(pz);
#endif
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double t_start = MPI_Wtime();

  TreeSoA local_tree(q, n);
  build_bvh(q, p, local_tree);
  q.wait();

  double t_build_local = MPI_Wtime() - t_start;

  // Identify and extract halo particles
  particles<coord_t> hp;
  for (int i = 0; i < n; ++i) {
    if (p.pos_x[i] >= halo_min && p.pos_x[i] <= halo_max) {
      hp.pos_x.push_back(p.pos_x[i]);
      hp.pos_y.push_back(p.pos_y[i]);
      hp.pos_z.push_back(p.pos_z[i]);
    }
  }

  int num_halo = hp.pos_x.size();

  MPI_Barrier(MPI_COMM_WORLD);
  t_start = MPI_Wtime();

  TreeSoA halo_tree(q, num_halo);
  if (num_halo > 0) { build_bvh(q, hp, halo_tree); }
  q.wait();

  double t_build_halo = MPI_Wtime() - t_start;

  // Exchange sizes
  uint64_t send_dims[2] = {static_cast<uint64_t>(halo_tree.num_leaves), static_cast<uint64_t>(halo_tree.num_internal)};
  uint64_t recv_dims[2];

  MPI_Barrier(MPI_COMM_WORLD);
  t_start = MPI_Wtime();

  MPI_Sendrecv(send_dims, 2, MPI_UINT64_T, neighbor, 0, recv_dims, 2, MPI_UINT64_T, neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  size_t recv_num_leaves = recv_dims[0];
  size_t recv_num_internal = recv_dims[1];
  size_t recv_total_nodes = recv_num_leaves + recv_num_internal;
  size_t send_total_nodes = halo_tree.num_leaves + halo_tree.num_internal;

  TreeSoA recv_tree(q, recv_num_leaves);

  // Helper to exchange SoA arrays
  auto exchange_array = [&](auto send_buf, int send_count, auto recv_buf, int recv_count, MPI_Datatype type, int tag) {
    MPI_Sendrecv(send_buf, send_count, type, neighbor, tag, recv_buf, recv_count, type, neighbor, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  };

  if (send_total_nodes > 0 || recv_total_nodes > 0) {
    exchange_array(halo_tree.min_x, send_total_nodes, recv_tree.min_x, recv_total_nodes, mpi_type_traits<coord_t>::type(), 1);
    exchange_array(halo_tree.max_x, send_total_nodes, recv_tree.max_x, recv_total_nodes, mpi_type_traits<coord_t>::type(), 2);
    exchange_array(halo_tree.min_y, send_total_nodes, recv_tree.min_y, recv_total_nodes, mpi_type_traits<coord_t>::type(), 3);
    exchange_array(halo_tree.max_y, send_total_nodes, recv_tree.max_y, recv_total_nodes, mpi_type_traits<coord_t>::type(), 4);
    exchange_array(halo_tree.min_z, send_total_nodes, recv_tree.min_z, recv_total_nodes, mpi_type_traits<coord_t>::type(), 5);
    exchange_array(halo_tree.max_z, send_total_nodes, recv_tree.max_z, recv_total_nodes, mpi_type_traits<coord_t>::type(), 6);
    exchange_array(halo_tree.parent, send_total_nodes, recv_tree.parent, recv_total_nodes, MPI_INT, 7);
  }

  if (halo_tree.num_internal > 0 || recv_num_internal > 0) {
    exchange_array(halo_tree.left_child, halo_tree.num_internal, recv_tree.left_child, recv_num_internal, MPI_INT, 8);
    exchange_array(halo_tree.right_child, halo_tree.num_internal, recv_tree.right_child, recv_num_internal, MPI_INT, 9);
  }

  q.wait();
  double t_transmission = MPI_Wtime() - t_start;

  // Verify the received tree via a query
  MPI_Barrier(MPI_COMM_WORLD);
  t_start = MPI_Wtime();

  if (recv_num_leaves > 0) {
    coord_t *dqx = sycl::malloc_shared<coord_t>(1, q);
    coord_t *dqy = sycl::malloc_shared<coord_t>(1, q);
    coord_t *dqz = sycl::malloc_shared<coord_t>(1, q);
    coord_t *drm = sycl::malloc_shared<coord_t>(1, q);
    coord_t *dRM = sycl::malloc_shared<coord_t>(1, q);
    int *res = sycl::malloc_shared<int>(10, q);
    int *res_cnt = sycl::malloc_shared<int>(1, q);

    // Query the center of the received halo
    dqx[0] = (rank == 0) ? static_cast<coord_t>(105.0) : static_cast<coord_t>(95.0);  // Center of neighbor's halo
    dqy[0] = static_cast<coord_t>(50.0);
    dqz[0] = static_cast<coord_t>(50.0);
    drm[0] = static_cast<coord_t>(0.0);
    dRM[0] = static_cast<coord_t>(5.0);
    res_cnt[0] = 0;

    range_query(q, recv_tree, dqx, dqy, dqz, drm, dRM, 1, res, res_cnt, 10);
    q.wait();

    sycl::free(dqx, q);
    sycl::free(dqy, q);
    sycl::free(dqz, q);
    sycl::free(drm, q);
    sycl::free(dRM, q);
    sycl::free(res, q);
    sycl::free(res_cnt, q);
  }

  double t_verify = MPI_Wtime() - t_start;

  // Output timings
  for (int r = 0; r < 2; ++r) {
    if (rank == r) {
      std::cout << "\n=== Rank " << rank << " Halo Exchange Report ===\n";
      std::cout << "  Local Particles        : " << n << "\n";
      std::cout << "  Halo Particles         : " << num_halo << "\n";
      std::cout << "  Received LET Leaves    : " << recv_num_leaves << "\n";
      std::cout << std::fixed << std::setprecision(4);
      std::cout << "  [Timing] Local Tree Build  : " << t_build_local * 1000.0 << " ms\n";
      std::cout << "  [Timing] Halo Tree Build   : " << t_build_halo * 1000.0 << " ms\n";
      std::cout << "  [Timing] MPI Transmission  : " << t_transmission * 1000.0 << " ms\n";
      std::cout << "  [Timing] Tree Verification : " << t_verify * 1000.0 << " ms\n";
      std::cout << "========================================\n";
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  local_tree.free(q);
  halo_tree.free(q);
  recv_tree.free(q);

  MPI_Finalize();
  return 0;
}