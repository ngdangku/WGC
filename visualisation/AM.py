import os

import numpy as np
import matplotlib.pyplot as plt
import networkx as nx


def parse_binary(binary_data):
    cur = 0
    total = len(binary_data)

    def read(type="int"):
        nonlocal cur
        size = 4
        if type == "long":
            size = 8
        res = int.from_bytes(binary_data[cur:cur + size], byteorder="little", signed=True)
        cur += size
        return res

    def read_all():
        size = 4
        res = []
        nonlocal cur
        while cur < total:
            res.append(int.from_bytes(binary_data[cur:cur + size], byteorder="little", signed=True))
            cur += size
        return res

    nodes = read()
    edges = read(type="long")
    graph = [[] for _ in range(nodes + 1)]
    print(nodes, edges)
    # graph = {}

    integer_sequence = read_all()
    print(integer_sequence[:10])

    cur_node = 0
    for i in integer_sequence:
        if i < 0:
            cur_node = i
            # graph[-cur_node] = []
        elif cur_node < 0:
            graph[-cur_node].append(i)

    return graph


def load_datasets():
    dataset_names = ["cnr-2000", "eu-2005"]
    dir = "../"

    datasets = []

    for name in dataset_names[:1]:
        path = os.path.join(dir, name)
        with open(path, "rb") as f:
            binary_data = f.read()
            parsed = parse_binary(binary_data)
            datasets.append(parsed)

    return datasets, dataset_names


datasets, dataset_names = load_datasets()


def visualise_dataset(AL, name):
    print("start visualising...")
    fig = plt.figure(figsize=(8,8))

    n = len(AL)
    x_list = []
    y_list = []
    for u, lis in enumerate(AL):
        d = len(lis)
        x_list.extend([u]*d)
        y_list.extend(lis)

    # plt.xlim(1,n)
    # plt.ylim(1,n)

    x = np.array(x_list)
    y = np.array(y_list)
    plt.scatter(x,y,1,c="grey")

    print("outputing...")

    plt.savefig("{}.am.png".format(name), dpi=300)


for data, dataset_name in zip(datasets, dataset_names):
    visualise_dataset(data, dataset_name)
    break
