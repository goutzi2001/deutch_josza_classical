//
// Created by goutzi on 2/10/23.
//

#include <bitset>
#include <iostream>
#include <queue>
#include <cmath>

/**
 * For every bit return always true.
 * Result for N Bits: 00000...000
 */
template <std::size_t N>
bool constant(const std::bitset<N>& bits) { return true; }

/**
 * For every bit 'flip' balanced_variable,
 * Result for N Bits: 01010...101
 */
template <std::size_t N>
bool balanced(const std::bitset<N>& bits)
{
    static bool balanced_variable = true;
    balanced_variable = !balanced_variable;
    return balanced_variable;
}

template<std::size_t N>
using Oracle = bool (*)(const std::bitset<N>&);


/**
 * An example of Deutch-Josza classical implementation of the oracle.
 * Full evaluation with N iterations.
 */
template<std::size_t N, Oracle<N> oracle>
std::pair<std::string, unsigned> deutch_josza()
{
    unsigned is_true = 0, is_false = 0, iterations = 0;
    int previous = -1;

    const long super_set = std::pow(2, N);

    const long max_const_limit = std::pow(2, N-1) + 1;

    for (std::size_t i = 0; i< super_set; ++i)
    {
        std::bitset<N> tmp;
        for (std::size_t j = 0; j< N; ++j)
            if ((i & (1 << j)) != 0)
                tmp[j] = true;

        ++iterations;
        std::cout << tmp << std::endl;
        const bool result = oracle(tmp);
        if(result)
            ++is_true;
        else
            ++is_false;

        if(is_true == max_const_limit || is_false == max_const_limit)
            return {"constant, early exit", iterations};

        // Early exit for balanced case
        if(previous != -1 && previous != static_cast<int>(result))
            return {"balanced, early exit", iterations};

        previous = static_cast<int>(result);
    }

    if(is_true == is_false)
        return {"balanced", iterations};

    return {"constant", iterations};
}


int main()
{
    constexpr std::size_t N = 4;

    std::cout << "Total possible combinations (power set): " << std::pow(2,N) <<std::endl;

    std::cout << "--------------------------\nExample of a constant function:\n";
    const auto& constant_function_test = deutch_josza<N, constant>();
    std::cout << "Evaluated:\t" << constant_function_test.first
              << ",\t Iterations: " << constant_function_test.second << std::endl;

    std::cout << "--------------------------\nExample of a balanced function:\n";
    const auto& balanced_function_test = deutch_josza<N, balanced>();
    std::cout << "Evaluated:\t" << balanced_function_test.first
              << ",\t Iterations: " << balanced_function_test.second << std::endl;
}
