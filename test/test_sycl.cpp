#include <sycl/sycl.hpp>
#include <iostream>

int main() {
  sycl::queue q;
  std::cout << "Running on: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
  q.parallel_for(sycl::range<1>(100), [=](sycl::id<1> i) {
     // dummy
   }).wait();
  std::cout << "Success" << std::endl;
  return 0;
}
