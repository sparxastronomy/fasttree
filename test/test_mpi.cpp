#include "../src/hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace fasttree;

// Dynamic MPI Datatype traits
template <typename T> struct mpi_type_traits {
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

template <> struct mpi_type_traits<float> {
    static MPI_Datatype type() { return MPI_FLOAT; }
};

template <> struct mpi_type_traits<double> {
    static MPI_Datatype type() { return MPI_DOUBLE; }
};

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) std::cerr << "MPI test requires at least 2 ranks.\n";
        MPI_Finalize();
        return 0;
    }

    sycl::queue q;
    const int   n = 1000;

    TreeSoA *tree_ptr = nullptr;

    if (rank == 0) {
        std::cout << "Rank 0: Building tree...\n";
        coord_t *sx  = sycl::malloc_shared<coord_t>(n, q);
        coord_t *sy  = sycl::malloc_shared<coord_t>(n, q);
        coord_t *sz  = sycl::malloc_shared<coord_t>(n, q);
        sfc_key *smk = sycl::malloc_shared<sfc_key>(n, q);

        // Generate simple data
        for (int i = 0; i < n; ++i) {
            sx[i]  = static_cast<coord_t>(i);
            sy[i]  = static_cast<coord_t>(0.0);
            sz[i]  = static_cast<coord_t>(0.0);
            smk[i] = static_cast<sfc_key>(i); // dummy sorted keys
        }

        tree_ptr = new TreeSoA(q, n);
        build_tree(q, *tree_ptr, smk, sx, sy, sz);
        q.wait();

        // Serialize metadata
        uint64_t dims[2] = {
            static_cast<uint64_t>(tree_ptr->num_leaves),
            static_cast<uint64_t>(tree_ptr->num_internal)
        };
        MPI_Send(dims, 2, MPI_UINT64_T, 1, 0, MPI_COMM_WORLD);

        size_t total_nodes = tree_ptr->num_leaves + tree_ptr->num_internal;

        // Serialize arrays
        MPI_Send(
            tree_ptr->min_x, total_nodes, mpi_type_traits<coord_t>::type(), 1, 1, MPI_COMM_WORLD
        );
        MPI_Send(
            tree_ptr->max_x, total_nodes, mpi_type_traits<coord_t>::type(), 1, 2, MPI_COMM_WORLD
        );
        MPI_Send(
            tree_ptr->min_y, total_nodes, mpi_type_traits<coord_t>::type(), 1, 3, MPI_COMM_WORLD
        );
        MPI_Send(
            tree_ptr->max_y, total_nodes, mpi_type_traits<coord_t>::type(), 1, 4, MPI_COMM_WORLD
        );
        MPI_Send(
            tree_ptr->min_z, total_nodes, mpi_type_traits<coord_t>::type(), 1, 5, MPI_COMM_WORLD
        );
        MPI_Send(
            tree_ptr->max_z, total_nodes, mpi_type_traits<coord_t>::type(), 1, 6, MPI_COMM_WORLD
        );

        MPI_Send(tree_ptr->left_child, tree_ptr->num_internal, MPI_INT, 1, 7, MPI_COMM_WORLD);
        MPI_Send(tree_ptr->right_child, tree_ptr->num_internal, MPI_INT, 1, 8, MPI_COMM_WORLD);
        MPI_Send(tree_ptr->parent, total_nodes, MPI_INT, 1, 9, MPI_COMM_WORLD);

        std::cout << "Rank 0: Tree sent successfully.\n";

        sycl::free(sx, q);
        sycl::free(sy, q);
        sycl::free(sz, q);
        sycl::free(smk, q);
    } else if (rank == 1) {
        std::cout << "Rank 1: Waiting for tree...\n";

        uint64_t dims[2];
        MPI_Recv(dims, 2, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        size_t num_leaves   = static_cast<size_t>(dims[0]);
        size_t num_internal = static_cast<size_t>(dims[1]);

        tree_ptr           = new TreeSoA(q, num_leaves);
        size_t total_nodes = num_leaves + num_internal;

        MPI_Recv(
            tree_ptr->min_x,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            1,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->max_x,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            2,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->min_y,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            3,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->max_y,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            4,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->min_z,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            5,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->max_z,
            total_nodes,
            mpi_type_traits<coord_t>::type(),
            0,
            6,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        MPI_Recv(
            tree_ptr->left_child, num_internal, MPI_INT, 0, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE
        );
        MPI_Recv(
            tree_ptr->right_child, num_internal, MPI_INT, 0, 8, MPI_COMM_WORLD, MPI_STATUS_IGNORE
        );
        MPI_Recv(tree_ptr->parent, total_nodes, MPI_INT, 0, 9, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Rank 1: Tree received successfully.\n";

        // Verify tree integrity by doing a range query
        coord_t qx = static_cast<coord_t>(500.5), qy = static_cast<coord_t>(0.0),
                qz    = static_cast<coord_t>(0.0);
        coord_t r_min = static_cast<coord_t>(0.0), r_max = static_cast<coord_t>(10.0);
        int     max_res = 100;

        coord_t *dqx     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqy     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqz     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *drm     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dRM     = sycl::malloc_shared<coord_t>(1, q);
        int     *res     = sycl::malloc_shared<int>(max_res, q);
        int     *res_cnt = sycl::malloc_shared<int>(1, q);

        dqx[0]     = qx;
        dqy[0]     = qy;
        dqz[0]     = qz;
        drm[0]     = r_min;
        dRM[0]     = r_max;
        res_cnt[0] = 0;

        range_query(q, *tree_ptr, dqx, dqy, dqz, drm, dRM, 1, res, res_cnt, max_res);
        q.wait();

        std::cout << "Rank 1: Found " << res_cnt[0] << " particles within radius " << r_max
                  << " of " << qx << std::endl;

        if (res_cnt[0] == 20) {
            std::cout << "Rank 1: SUCCESS! MPI LET transferred correctly." << std::endl;
        } else {
            std::cout << "Rank 1: FAILURE! Expected 20 particles." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        sycl::free(dqx, q);
        sycl::free(dqy, q);
        sycl::free(dqz, q);
        sycl::free(drm, q);
        sycl::free(dRM, q);
        sycl::free(res, q);
        sycl::free(res_cnt, q);
    }

    if (tree_ptr) {
        tree_ptr->free(q);
        delete tree_ptr;
    }

    MPI_Finalize();
    return 0;
}