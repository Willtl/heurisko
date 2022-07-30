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

namespace reader
{
    std::string instancesPath;
    enum TspInstance
    {
        A280,
        EIL51,
        EIL76,
        VLSI_XQF131,
        VLSI_XQG237,
        VLSI_PMA343,
        VLSI_XQL662,
        VLSI_BBZ25234 
    };

    std::map<TspInstance, std::string> instanceNames =
        {
            {TspInstance::A280, "a280.tsp"},
            {TspInstance::EIL51, "eil51.tsp"},
            {TspInstance::EIL76, "eil76.tsp"},
            {TspInstance::VLSI_XQF131, "vlsi/xqf131.tsp"},
            {TspInstance::VLSI_XQG237, "vlsi/xqg237.tsp"},
            {TspInstance::VLSI_PMA343, "vlsi/pma343.tsp"},
            {TspInstance::VLSI_XQL662, "vlsi/xql662.tsp"},
            {TspInstance::VLSI_BBZ25234, "vlsi/bbz25234.tsp"}
        };

    std::vector<std::vector<float>> distanceMatrix;

    bool existsFileAtPath(const std::filesystem::path &p, std::filesystem::file_status s = std::filesystem::file_status{})
    {
        if (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p))
            return true;
        return false;
    }

    void checkInstanceFolder()
    {
        std::string currentPath = std::filesystem::current_path().u8string();

        std::filesystem::path instancesPath = std::filesystem::u8path(currentPath + "/../instances");

        std::string formatedCurrentPath = currentPath;
        std::replace(formatedCurrentPath.begin(), formatedCurrentPath.end(), '\\', '/');
        std::string formatedInstancesPath = instancesPath.u8string();
        std::replace(formatedInstancesPath.begin(), formatedInstancesPath.end(), '\\', '/');

        ps << formatedInstancesPath << pe;

        if (existsFileAtPath(instancesPath))
        {
            std::cout << "Instances folder found at " << formatedInstancesPath << std::endl;
            reader::instancesPath = formatedInstancesPath;
        }
        else
        {
            std::cerr << "Failed to load instances from: " << formatedCurrentPath << std::endl;
            exit(1);
        }
    }

    std::vector<std::string> split(const std::string &str, char delim)
    {
        std::vector<std::string> strings;
        size_t start;
        size_t end = 0;
        while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
        {
            end = str.find(delim, start);
            strings.push_back(str.substr(start, end - start));
        }
        return strings;
    }

    float euclideanDistance(float x1, float y1, float x2, float y2) { return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); }

    void calculateDistances(const std::vector<std::pair<int, int>> &rawNodes, std::vector<std::vector<int>> &distanceMatrix)
    {
        ps << "Creating distance matrix" << pe;
        const int dimension = rawNodes.size();
        distanceMatrix = std::vector<std::vector<int>>(dimension, std::vector<int>(dimension, 0));
        
        ps << "Computing distance matrix" << pe;
        #pragma omp parallel for
        for (size_t i = 0; i < dimension; i++)
            for (size_t j = 0; j < dimension; j++)
                distanceMatrix[i][j] = round(euclideanDistance(rawNodes[i].first, rawNodes[i].second, rawNodes[j].first, rawNodes[j].second));
    }

    std::vector<std::vector<int>> readTspInstance(TspInstance instance)
    {
        // Check if instances folder is placed at correct path
        reader::checkInstanceFolder();

        // Define instance path
        std::string instanceName = reader::instanceNames[instance];
        std::string instancePath = reader::instancesPath + "/tsp/" + instanceName;

        if (reader::existsFileAtPath(std::filesystem::u8path(instancePath)))
            std::cout << instancePath << std::endl;

        // Load instance file
        std::ifstream file(instancePath);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::vector<std::string> lines = split(content, '\n');

        std::vector<std::pair<int, int>> nodes;
        for (auto line : lines)
        {
            std::vector<std::string> values = split(line, ' ');
            int x = std::stod(values[1]);
            int y = std::stod(values[2]);
            std::pair<int, int> coordinate = std::make_pair(x, y);
            nodes.push_back(coordinate);
        }
        ps << "Number of nodes:  " << nodes.size() << pe;

        std::vector<std::vector<int>> distanceMatrix;
        calculateDistances(nodes, distanceMatrix);
        return distanceMatrix;
    }
} // namespace reader

#endif // TSPREADER_H
