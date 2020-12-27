#include "reduce.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " num" << std::endl;
        return 0;
    }

    // init random seed
    srand(time(NULL));

    int num = std::stoi(argv[1]);
    std::vector<float> vec(num);
    for (int i = 0; i < num; ++i)
    {
        vec[i] = rand() * 1.0f / RAND_MAX;
    }

    int reduce_size = 384;
    std::vector<float> vec_out_thread, vec_out_cuda;
    reduce_thread(vec, reduce_size, vec_out_thread);
    reduce_cuda(vec, reduce_size, vec_out_cuda);

    bool ret = compare(vec_out_thread, vec_out_cuda);
    std::cout << (ret ? "PASSED!" : "FAILED!") << std::endl;

    return 0;
}

