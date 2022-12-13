import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs
from os import listdir

datasets_directory = "./../datasets/"
clustering_results_directory = "./../clustering_results/"


def generate_2d_clustering_dataset_csv(n_samples: int, centers: int, directory: str) -> None:
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


def read_from_csv_and_plot(path: str, delimiter: str) -> None:
    points = np.genfromtxt(fname=path, delimiter=delimiter)
    plt.clf()
    plt.scatter(points[:, 0], points[:, 1], c=points[:, 2])
    plt.savefig(f'{path[:-4]}.png')


if __name__ == '__main__':
    # for i in [100000, 200000, 300000, 400000, 500000]:
    #     generate_2d_clustering_dataset_csv(i, 4, datasets_directory)

    # for filename in listdir(clustering_results_directory):
    #     if filename[-3:] == "csv":
    #         read_from_csv_and_plot(clustering_results_directory + filename, ',')

    # n 100000, k 1000
    points = np.array([(1, 12.1057), (2, 7.8597), (4, 5.25893), (8, 4.33545), (16, 4.49782)])
    points[:, 1] = points[0, 1] / points[:, 1]

    # n 100000, k 100
    points2 = np.array([(1, 1.45824), (2, 0.969685), (4, 0.704211), (8, 0.473201), (16, 0.4979)])
    points2[:, 1] = points2[0, 1] / points2[:, 1]

    # n 100000, k 10000
    points3 = np.array([(1, 126.525), (2, 85.5906), (4, 69.0547), (8, 65.5702), (16, 68.286)])
    points3[:, 1] = points3[0, 1] / points3[:, 1]

    # n 100000, k 10
    points4 = np.array([(1, 0.213202), (2, 0.193648), (4, 0.173003), (8, 0.149905), (16, 0.202764)])
    points4[:, 1] = points4[0, 1] / points4[:, 1]

    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.axis([1, 17, 1, 4])
    plt.title('Speedup with different K')
    plt.plot(points4[:, 0], points4[:, 1], color='m', label='K = 10')
    plt.plot(points2[:, 0], points2[:, 1], color='b', label='K = 100')
    plt.plot(points[:, 0], points[:, 1], color='r', label='K = 1000')
    plt.plot(points3[:, 0], points3[:, 1], color='g', label='K = 10000')
    plt.legend()
    plt.savefig('speedup.png')
