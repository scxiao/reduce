#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <thread>
#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cmath>

template <class T>
void print_vec(std::ostream& os, const std::vector<T>& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        if (i != 0) 
            os << ", ";
        else
            os << "{";
        if ((i% 8) == 0 and i != 0) os << std::endl;
        os << std::setw(12) << vec[i];
    }
    os << "}";
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& vec)
{
    print_vec<T>(os, vec);
    return os;
}

namespace detail {
template<class T, bool = std::is_integral<T>::value>
struct helper { };

template<class T>
struct helper<T, false>
{
    static bool compare(const std::vector<T>& v1, const std::vector<T>& v2)
    {
        T eps = 0.01;
        if (v1.size() != v2.size())
        {
            std::cout << "elem num mismatch!" << std::endl;
            return false;
        }

        T max_delta = eps;
        int max_index = -1;
        for (int i = 0; i < v1.size(); ++i)
        {
            if (v1[i] - v2[i] > eps or v1[i] - v2[i] < -eps)
            {
                std::cout << "(v1[" << i << "] = " << v1[i] << ") != ";
                std::cout << "(v2[" << i << "] = " << v2[i] << ")" << std::endl;
                if (max_delta < std::fabs(v1[i] - v2[i]))
                {
                    max_delta = std::fabs(v1[i] - v2[i]);
                    max_index = i;
                }
            }
        }

        if (max_index != -1)
        {
            std::cout << "max_diff = " << max_delta << ", corresponding values:" << std::endl;
            int i = max_index;
            std::cout << "(v1[" << i << "] = " << v1[i] << ") != ";
            std::cout << "(v2[" << i << "] = " << v2[i] << ")" << std::endl;

            return false;
        }

        return true;
    }
};

template<class T>
struct helper<T, true>
{
    static bool compare(const std::vector<T>& v1, const std::vector<T>& v2)
    {
        if (v1.size() != v2.size())
        {
            std::cout << "elem num mismatch!" << std::endl;
            return false;
        }

        bool is_same = true;
        for (int i = 0; i < v1.size(); ++i)
        {
            if (v1[i] != v2[i])
            {
                std::cout << "(v1[" << i << "] = " << v1[i] << ") != ";
                std::cout << "(v2[" << i << "] = " << v2[i] << ")" << std::endl;
                is_same = false;
            }
        }

        return is_same;
    }
};

}

template<class T>
bool compare(const std::vector<T>& v1, const std::vector<T>& v2)
{
    return detail::helper<T>::compare(v1, v2);
}

#endif

