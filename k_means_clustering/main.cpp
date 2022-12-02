#include <iostream>
#include "kMeansClustering.h"
#include "utils.h"

std::string datasetsDirectory = "./../datasets/";
std::string clusteringResultsDirectory = "./../clustering_results/";

int n = 1;

int main() {

    std::vector<std::string> datasetsFilenames = getDatasetFilenames(datasetsDirectory);

    for (const auto& filename : datasetsFilenames) {
        std::cout << "Reading csv" << std::endl;
        std::vector<Point> points = readPointsFromCsv(datasetsDirectory + filename, ',');
        std::cout << "Reading csv finished" << std::endl;

        std::vector<Cluster> clusters;

        for (int t = 1; t < 17; t++) {
            omp_set_num_threads(t);

            double start = omp_get_wtime();
            for (int i = 0; i < n; i++) {
                clusters = kMeansClustering(4, points);
            }
            double end = omp_get_wtime();
            double time = (end - start) / n;
            std::cout << "Total time for file " << filename << ": " << time << " s with " << t << " threads and " << clusters.size() << " clusters" << std::endl;
        }

//        writeClustersToCsv(clusters, clusteringResultsDirectory + filename, ',');
    }

    return 0;
}