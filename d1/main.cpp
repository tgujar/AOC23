#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <array>

std::vector<std::string> readlines(const std::string &filename)
{
    using namespace std;
    fstream is{filename};
    vector<string> lines;
    if (is.is_open())
    {
        string line;
        while (getline(is, line))
        {
            lines.push_back(line);
        }
    }
    else
    {
        cerr << "Cannot open file";
        exit(1);
    }
    return lines;
}

std::pair<int, int> get_first_last_num(const std::string &s)
{
    size_t start = s.size(), end = -1;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (std::isdigit(s[i]))
        {
            if (start > i)
                start = i;
            end = i;
        }
    }
    return std::make_pair(start, end);
}

struct spelled_digit
{
    size_t pos;
    int val;
};

std::pair<spelled_digit, spelled_digit>
get_first_last_num_spelled(const std::string &s)
{
    std::array<std::string, 9> digits{
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    spelled_digit start{std::string::npos, -1};
    spelled_digit end{std::string::npos, -1};
    for (int i = 0; i < 9; i++)
    {
        size_t fpos = s.find(digits[i]);
        size_t lpos = s.rfind(digits[i]);
        if (fpos != std::string::npos && start.pos > fpos)
        {
            start.pos = fpos;
            start.val = i + 1;
        }
        if (lpos != std::string::npos &&
            (end.pos == std::string::npos || end.pos < lpos))
        {
            end.pos = lpos;
            end.val = i + 1;
        }
    }
    return std::make_pair(start, end);
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Must provide file path to read";
        exit(1);
    }
    std::string filename{argv[1]};
    std::vector<std::string> lines{readlines(filename)};
    std::vector<int> nums_v1, nums_v2;
    std::transform(lines.cbegin(), lines.cend(), std::back_inserter(nums_v1),
                   [](const auto &line)
                   {
                       auto [itens, iones] = get_first_last_num(line);
                       return (line[itens] - '0') * 10 + (line[iones] - '0');
                   });

    std::transform(lines.cbegin(), lines.cend(), std::back_inserter(nums_v2),
                   [](const auto &line)
                   {
                       auto [itens, iones] = get_first_last_num(line);
                       auto [s_tens, s_ones] = get_first_last_num_spelled(line);
                       int tens = -1, ones = -1;
                       if (s_tens.pos < itens)
                       {
                           tens = s_tens.val;
                       }
                       else
                       {
                           tens = line[itens] - '0';
                       }

                       /* no spelled number can start at the maximum possible
                        position since all numbers have more than one letter
                        */
                       if (s_ones.pos == std::string::npos || iones > s_ones.pos)
                       {
                           ones = line[iones] - '0';
                       }
                       else
                       {
                           ones = s_ones.val;
                       }
                       return tens * 10 + ones;
                   });
    std::cout
        << std::accumulate(nums_v1.cbegin(), nums_v1.cend(), 0) << "\n";
    std::cout
        << std::accumulate(nums_v2.cbegin(), nums_v2.cend(), 0) << "\n";
}