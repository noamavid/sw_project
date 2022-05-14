import numpy as np
import pandas as pd
import sys

DEFAULT_MAX_ITER = 100
DEBUG = True


def print_error():
    print("An Error Has Occurred\n")


def print_invalid_input():
    print("Invalid Input!\n")


def get_program_args():
    try:
        assert 5 <= len(sys.argv) <= 6
        k = int(sys.argv[1])
        filename1 = sys.argv[-2]
        filename2 = sys.argv[-1]
        eps = float(sys.argv[-3])
        if len(sys.argv) == 6:
            max_iter = int(sys.argv[2])
        else:
            max_iter = DEFAULT_MAX_ITER
    except:
        print_invalid_input()
        return False
    return [k, max_iter, eps, filename1, filename2]


def read_data(filename1, filename2):
    df1 = pd.read_csv(filename1, header=None)
    df1.columns= [str(i) for i in range(len(df1.columns))]
    df2 = pd.read_csv(filename2, header=None)
    df2.columns = [str(i) for i in range(len(df2.columns))]
    res = pd.merge(df1, df2, on="0")
    res.sort_values( "0", inplace= True)
    res.drop("0", inplace=True, axis=1)
    return res.to_numpy()


def initialize_centroids(vectors: np.ndarray, k: int):
    np.random.seed(0)
    N = vectors.shape[0]
    centroids_indices = np.zeros(k, dtype="int32")
    centroids_indices[0] = np.random.randint(0, N)
    dists = np.zeros(N)
    probs = np.zeros(N)
    for i in range(1, k):
        for l in range(N):
            d = [np.square(np.linalg.norm(vectors[l] - vectors[centroids_indices[j]])) for j in range(i)]
            dists[l] = np.min(d)
        probs = dists / np.sum(dists)
        centroids_indices[i] = np.random.choice(N, p=probs)
    return centroids_indices


def create_c_input(vectors: np.ndarray, centroids_indices: np.ndarray):
    """
    This function creates a list of vectors (initialized centorids first) and writes it to file
    :param vectors: np.ndarray
    :param centroids_indices:np.ndarray
    :return: None
    """
    N = vectors.shape[0]
    try:
        sorted_vectors = [list(vectors[i]) for i in centroids_indices] + [list(vectors[i]) for i in range(N) if i not in centroids_indices]
        sorted_vectors = [[format(x, '.4f') for x in i] for i in sorted_vectors]
        with open("c_input_file", 'w+') as c_input:
            for vec in sorted_vectors:
                c_input.write(",".join(vec) + "\n")
    except Exception as err:
        if DEBUG:
            print(err)
            print("problem in 'create c input'")
        print_error()


def main():
    # print(get_program_args(), "\n")
    np.random.seed(0)
    k = 3
    N = 100
    d = 7
    # create_c_input(vectors, centroids_indices)

    centroids_indices = initialize_centroids(read_data("input_1_db_1.txt", "input_1_db_2.txt"), k)
    print("indices: ", centroids_indices)



if __name__ == '__main__':
    main()
