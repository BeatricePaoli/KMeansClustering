#include <iostream>
#include <omp.h>
#include "kMeansClustering.h"
#include "utils.h"

std::string datasetsDirectory = "./../datasets/";
std::string clusteringResultsDirectory = "./../clustering_results/";

int n = 1;
int k = 4;
int min_t = 8;
int max_t_ex = 9;

int main() {

    std::vector<std::string> datasetsFilenames = getDatasetFilenames(datasetsDirectory);

    for (const auto& filename : datasetsFilenames) {
        std::cout << "Reading csv" << std::endl;
        Points points = readPointsStructFromCsv(datasetsDirectory + filename, ',');
        std::cout << "Reading csv finished" << std::endl;

        Clusters clusters;

        for (int t = min_t; t < max_t_ex; t++) {
            omp_set_num_threads(t);

            double start = omp_get_wtime();
            for (int i = 0; i < n; i++) {
                clusters = kMeansClustering(k, points);
            }
            double end = omp_get_wtime();
            double time = (end - start) / n;
            std::cout << "Total time for file " << filename << ": " << time << " s with " << t << " threads and " << clusters.size << " clusters" << std::endl;
        }

//        writePointsStructToCsv(points, clusteringResultsDirectory + filename, ',');
    }

    return 0;
}