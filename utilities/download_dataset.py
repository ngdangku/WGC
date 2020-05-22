""" download_dataset.py
Copyright (C) 2020, Dengke Wu

Download dataset

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

import logging

from .webgraph_dataset import WebGraphDatasetManager

logging.basicConfig(level=logging.INFO)


def main():
    dataset_names = [
        "cnr-2000",
        "in-2004",
        "eu-2005",
        "uk-2007-05@100000",
        "uk-2007-05@1000000"
    ]
    manager = WebGraphDatasetManager(dataset_names)
    manager.download_all_datasets()


if __name__ == '__main__':
    main()
