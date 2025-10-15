#include "util.h"
#include <cmath>
#include <vector>


std::vector<std::vector<int>> generateIndices(size_t level) {
    std::vector<std::vector<int>> indices(level);
    int start{ 0 };
    int end = static_cast<int>(std::pow(2, level)) - 1;
    int step_index{ 2 };
    int stepVector{ 1 };

    for(std::vector<int>& indices_level : indices) {
        for (int i = start; i <  end; i+=step_index) {
            indices_level.push_back(i);
        }
        start += stepVector;
        end += stepVector;
        stepVector *= 2;
        step_index *= 2;

    }
    
    return indices;
}