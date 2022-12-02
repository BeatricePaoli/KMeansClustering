import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from os import listdir

datasets_directory = "./../datasets/"
clustering_results_directory = "./../clustering_results/"


def generate_2d_clustering_dataset_csv(n_samples, centers, directory):
    (Xs, ys) = make_blobs(n_samples=n_samples, n_features=2, centers=centers, center_box=(-15.0, 15.0))
    np.savetxt(
        fname=f'{directory}{n_samples}_points_{centers}_centers.csv',
        X=Xs,
        fmt='%s',
        delimiter=','
    )
    plt.clf()
    plt.scatter(Xs[:, 0], Xs[:, 1], c=ys)
    plt.savefig(f'{directory}{n_samples}_points_{centers}_centers.png')


def read_from_csv_and_plot(path, delimiter):
    points = np.genfromtxt(fname=path, delimiter=delimiter)
    plt.clf()
    plt.scatter(points[:, 0], points[:, 1], c=points[:, 2])
    plt.scatter(points[:, 3], points[:, 4], c='r', marker='v')
    plt.savefig(f'{path[:-4]}.png')


if __name__ == '__main__':
    # for i in [100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000]:
    #     generate_2d_clustering_dataset_csv(i, 4, datasets_directory)

    # for i in range(5, 25, 5):
    #     generate_2d_clustering_dataset_csv(100000, i, datasets_directory)

    for filename in listdir(clustering_results_directory):
        if filename[-3:] == "csv":
            read_from_csv_and_plot(clustering_results_directory + filename, ',')
