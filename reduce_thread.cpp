#include <thread>
#include "reduce.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>

void reduce_t(float *buf, int elem_num, int reduce_size, int tid, int t_num, float* buf_out)
{
    for (int idx = tid; idx < elem_num / reduce_size; idx += t_num)
    {
        int sidx = idx * reduce_size;
        buf_out[idx] = 0.0f;
        for (int j = 0; j < reduce_size; j++)
        {
            buf_out[idx] += buf[sidx + j];
        }
    }
}

void reduce_thread(const std::vector<float>& vec, int reduce_size, std::vector<float>& vec_out)
{
    int t_num = std::thread::hardware_concurrency();
    std::cout << "thread_num = " << t_num << std::endl;
    //std::cout << "vec = " << std::endl;
    //std::cout << vec << std::endl;
    int elem_num = vec.size();
    int block_num = elem_num / reduce_size;
    vec_out.resize(block_num);

    std::vector<std::thread> vec_t;
    for (int i = 0; i < t_num; ++i)
    {
        vec_t.push_back(std::thread(reduce_t, (float*)vec.data(), vec.size(), reduce_size, i, t_num,
                    (float*)vec_out.data()));
    }

    for (auto& t : vec_t)
    {
        t.join();
    }
}

