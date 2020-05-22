""" webgraph_dataset.py
Copyright (C) 2020, Dengke Wu

Classes of webgraph dataset and its dataset manager

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
import os

import requests

# default configurations
EXTENSIONS = [
    ".properties",
    ".graph",
    ".md5sums",
]
BASE_URL = "http://data.law.di.unimi.it/webdata/"


class WebGraphDataset:
    def __init__(self, name, base_dir, extensions=None, base_url=BASE_URL):
        self.base_dir = base_dir
        self.extensions = extensions
        if self.extensions is None:
            self.extensions = EXTENSIONS.copy()
        self.name = name
        self.url_template = base_url + "{name}/{file_name}"

        if not os.path.exists(self.base_dir):
            os.mkdir(self.base_dir)

    def get_urls(self):
        return [self.url_template.format(name=self.name, file_name=file_name) for file_name in self.get_file_names()]

    def get_file_names(self):
        return [self.name + ext for ext in self.extensions]

    def get_paths(self):
        return [os.path.join(self.base_dir, file_name) for file_name in self.get_file_names()]

    def check(self):
        md5sum_files = [path for path in self.get_paths() if path.endswith("md5sums")]

        if len(md5sum_files) == 1:
            md5sum_file_path = md5sum_files[0]
            if os.path.exists(md5sum_file_path):
                is_check_failed = any([not os.path.exists(path) for path in self.get_paths()])
            else:
                is_check_failed = True
        else:
            is_check_failed = True
        return not is_check_failed

    def check_and_download(self, invoked_times=0):
        if invoked_times > 5:
            return False

        if self.check() is False:
            logging.warning("check failed, downloading {name}".format(name=self.name))
            self.download()
            return self.check_and_download(invoked_times + 1)

        return True

    def download(self, ):
        for url, path in zip(self.get_urls(), self.get_paths()):
            logging.info("downloading {url}".format(url=url))
            r = requests.get(url)
            with open(path, "wb") as f:
                f.write(r.content)
            logging.info("save to {path}".format(path=os.path.abspath(path)))

    # def convert(self):
    #
    #     return True


class WebGraphDatasetManager:

    def __init__(self, dataset_names, **kwargs):
        self.base_dir = "./webgraph_datasets"
        self.dataset_names = dataset_names

        for k, v in kwargs.items():
            self.__setattr__(k, v)

        self.datasets = [WebGraphDataset(name, self.base_dir, ) for name in self.dataset_names]

    def download_all_datasets(self, ):
        for dataset in self.datasets:
            result = dataset.check_and_download()
            if result:
                logging.info("{dataset_name} verified.".format(dataset_name=dataset.name))
            else:
                logging.error("{dataset_name} failed.".format(dataset_name=dataset.name))
        return self

    # convert all downloaded datasets to uncompressed binary form
    # def convert_all_datasets(self):
    #     logging.info("converting...")
    #     # i = int.from_bytes(b'\x00\x0F', byteorder="little", signed=True)
    #     results = ["Failed", "OK"]
    #     for dataset in self.datasets:
    #         is_ok = dataset.convert()
    #         logging.info("{dataset_name}: {result_str}".format(dataset_name=dataset.name, result_str=results[is_ok]))
    #
    #     logging.info("converted.")
