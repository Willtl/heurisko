#ifndef TSPREADER_H
#define TSPREADER_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

namespace reader {
std::string instancesPath;
enum TspInstance { A280, EIL51, EIL76, XQF131 };
std::map<TspInstance, std::string> instanceNames = {{TspInstance::A280, "a280.tsp"}, {TspInstance::EIL51, "eil51.tsp"}, {TspInstance::EIL76, "eil76.tsp"}, {TspInstance::XQF131, "xqf131.tsp"}};
std::vector<std::vector<double>> distanceMatrix;

bool existsFileAtPath(const std::filesystem::path &p, std::filesystem::file_status s = std::filesystem::file_status{}) {
    if (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p))
        return true;
    return false;
}

bool existsInstancesFolder() {
    std::string currentPath = std::filesystem::current_path().u8string();
    std::filesystem::path instancesPath = std::filesystem::u8path(currentPath + "\\instances\\");

    std::string formatedCurrentPath = currentPath;
    std::replace(formatedCurrentPath.begin(), formatedCurrentPath.end(), '\\', '/');
    std::string formatedInstancesPath = instancesPath.u8string();
    std::replace(formatedInstancesPath.begin(), formatedInstancesPath.end(), '\\', '/');

    if (existsFileAtPath(instancesPath)) {
        std::cout << "Instances loaded from " << formatedInstancesPath << std::endl;
        reader::instancesPath = formatedInstancesPath;
    } else {
        std::cerr << "You must copy the \"heuriskos/examples/instances\" folder into " << formatedCurrentPath << std::endl;
    }
}

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

double euclideanDistance(double x1, double y1, double x2, double y2) { return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); }

void calculateDistances(const std::vector<std::pair<double, double>> &rawNodes, std::vector<std::vector<double>> &distanceMatrix) {
    const int dimension = rawNodes.size();
    distanceMatrix = std::vector<std::vector<double>>(dimension, std::vector<double>(dimension, 0));
    for (size_t i = 0; i < dimension; i++)
        for (size_t j = 0; j < dimension; j++)
            distanceMatrix[i][j] = round(euclideanDistance(rawNodes[i].first, rawNodes[i].second, rawNodes[j].first, rawNodes[j].second));
}

std::vector<std::vector<double>> readTspInstance(TspInstance instance) {
    // Check if instances folder is placed at correct path
    reader::existsInstancesFolder();

    // Define instance path
    std::string instanceName = reader::instanceNames[instance];
    std::string instancePath = reader::instancesPath + "/tsp/" + instanceName;
    if (reader::existsFileAtPath(std::filesystem::u8path(instancePath))) {
        std::cout << instancePath << std::endl;
    }

    // Load instance file
    std::ifstream file(instancePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<std::string> lines = split(content, '\n');

    std::vector<std::pair<double, double>> nodes;
    for (auto line : lines) {
        std::vector<std::string> values = split(line, ' ');
        double x = std::stod(values[1]);
        double y = std::stod(values[2]);
        std::pair<double, double> coordinate = std::make_pair(x, y);
        nodes.push_back(coordinate);
    }

    std::vector<std::vector<double>> distanceMatrix;
    calculateDistances(nodes, distanceMatrix);
    return distanceMatrix;
}
} // namespace reader

#endif // TSPREADER_H
