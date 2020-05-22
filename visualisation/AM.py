""" AM.py
Copyright (C) 2020, Dengke Wu

Load and visualise the AM of datasets

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


"""
import os

import matplotlib.pyplot as plt
import numpy as np


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
    print("nodes:{}, edges:{}".format(nodes, edges))

    integer_sequence = read_all()

    cur_node = 0
    for i in integer_sequence:
        if i < 0:
            cur_node = i
        elif cur_node < 0:
            graph[-cur_node].append(i)

    return graph


def visualise_dataset(AL, name):
    print("start visualising...")
    fig = plt.figure()

    f, ax = plt.subplots(1)

    ax.set_aspect("equal")

    n = len(AL)
    x_list = []
    y_list = []
    for u, lis in enumerate(AL):
        d = len(lis)
        x_list.extend([u] * d)
        y_list.extend(lis)

    x = np.array(x_list)
    y = np.array(y_list)
    plt.scatter(x, y, 1, c="grey")

    print("outputing...")

    plt.xticks([1, n])
    plt.yticks([1, n])

    plt.title("{}".format(name))
    plt.xlabel("to")
    plt.ylabel("from")

    plt.savefig("{}.am.png".format(name), dpi=300)


def main():
    dataset_names = ["cnr-2000", "eu-2005","in-2004","uk-2007-05@1000000"]
    dir = "webgraph_datasets"

    for name in dataset_names:
        path = os.path.join(dir, name)
        print("loading {}...".format(name))
        with open(path, "rb") as f:
            binary_data = f.read()
            parsed = parse_binary(binary_data)
        visualise_dataset(parsed, name)


if __name__ == '__main__':
    main()
