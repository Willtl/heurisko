#ifndef TSPREADER_H
#define TSPREADER_H

#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

namespace reader {
std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::vector<std::pair<int, int>> readTSPInstance(std::string path) {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<std::string> lines = split(content, '\n');

    std::vector<std::pair<int, int>> nodes;
    for (auto line : lines) {
        std::vector<std::string> values = split(line, ' ');
        int x = std::stoi(values[1]);
        int y = std::stoi(values[2]);
        std::pair<int, int> coordinate = std::make_pair(x, y);
        nodes.push_back(coordinate);
    }
    return nodes;
}
} // namespace reader

#endif // TSPREADER_H
