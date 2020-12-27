#ifndef __REDUCE_HPP__
#define __REDUCE_HPP__
#include <vector>

void reduce_thread(const std::vector<float>& vec, int reduce_size, std::vector<float>& vec_out);
void reduce_cuda(const std::vector<float>& vec, int reduce_size, std::vector<float>& vec_out);

#endif

