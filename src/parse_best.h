/**
 *  @autor tekatod
 *  @date 3/14/17
 */
#pragma once

#include <string>
#include <fstream>
#include <sstream>

double read_score(std::string file_name) {
    std::ifstream f;
    f.open(file_name);
    std::string line;
    for(size_t i = 0; i < 3; ++i) {
        std::getline(f, line);
    }
    std::stringstream iss(line);
    std::string tmp;
    for (size_t i = 0; i < 2; ++i) {
        std::getline(iss, tmp, ' ');
    }
    double score = std::stod(tmp);
    return score;
}