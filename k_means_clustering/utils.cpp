#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include "utils.h"

std::vector<std::string> getDatasetFilenames(const std::string& datasetsDirectory) {
    std::vector<std::string> datasetsFilenames;
    for (const auto & entry : std::filesystem::directory_iterator(datasetsDirectory)) {
        std::string filename(entry.path().string());
        filename.erase(filename.find(datasetsDirectory), datasetsDirectory.length());
        std::string ext = filename.substr(filename.size() - 3);
        if (ext == "csv") {
            datasetsFilenames.push_back(filename);
        }
    }

    std::sort(datasetsFilenames.begin(), datasetsFilenames.end());
    return datasetsFilenames;
}

std::vector<Point> readPointsFromCsv(const std::string &filename, char delimiter) {
    std::vector<Point> points;
    std::vector<float> row;
    std::string line, word;

    std::fstream file(filename, std::ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();
            std::stringstream str(line);

            while (getline(str, word, delimiter))
                try {
                    row.push_back(std::stof(word));
                } catch (...) {
                    std::cout << "Unable to convert csv values to float" << std::endl;
                }
            if (row.size() >= 2) {
                points.emplace_back(row[0], row[1]);
            }
        }
    } else
        std::cout << "Could not open the file\n" << std::endl;
    return points;
}

//void writeClustersToCsv(const std::vector<Cluster> &clusters, const std::string &filename, char delimiter) {
//    std::fstream file(filename, std::ios::out);
//    if (file.is_open()) {
//        for (const auto &cluster: clusters) {
//            for (auto point: cluster.points) {
//                file << point.x << delimiter << point.y << delimiter << cluster.id << delimiter << cluster.centroid.x << delimiter << cluster.centroid.y << '\n';
//            }
//        }
//    } else
//        std::cout << "Could not open the file\n" << std::endl;
//}

void writePointsToCsv(const std::vector<Point> &points, const std::string &filename, char delimiter) {
    std::fstream file(filename, std::ios::out);
    if (file.is_open()) {
        for (auto point: points) {
            file << point.x << delimiter << point.y << delimiter << point.clusterId << '\n';
        }
    } else
        std::cout << "Could not open the file\n" << std::endl;
}