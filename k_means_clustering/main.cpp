#include <iostream>
#include <omp.h>
#include "kMeansClustering.h"
#include "utils.h"

std::string datasetsDirectory = "./../datasets/";
std::string clusteringResultsDirectory = "./../clustering_results/";

int n = 1;
int k = 1000;
int min_t = 1;
int max_t_ex = 17;

int main() {

    std::vector<std::string> datasetsFilenames = getDatasetFilenames(datasetsDirectory);

    for (const auto& filename : datasetsFilenames) {
        std::cout << "Reading csv" << std::endl;
        std::vector<Point> points = readPointsFromCsv(datasetsDirectory + filename, ',');
        std::cout << "Reading csv finished" << std::endl;

        std::vector<Cluster> clusters;
        std::vector<std::pair<int, double>> threadTimes;

        for (int t = min_t; t < max_t_ex; t*=2) {
            omp_set_num_threads(t);

            double start = omp_get_wtime();
            for (int i = 0; i < n; i++) {
                clusters = kMeansClustering(k, points);
            }
            double end = omp_get_wtime();
            double time = (end - start) / n;
            threadTimes.emplace_back(t, time);
            std::cout << "Total time for file " << filename << ": " << time << " s with " << t << " threads and " << clusters.size() << " clusters" << std::endl;
        }

        std::cout << "Thread-Times: ";
        for (auto tt : threadTimes) {
            std::cout << "(" << tt.first << ", " << tt.second << "), ";
        }
        std::cout << std::endl;

//        writePointsToCsv(points, clusteringResultsDirectory + filename, ',');
    }

    return 0;
}