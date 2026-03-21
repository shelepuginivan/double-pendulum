from dataclasses import dataclass
import sys
import math
from typing import Iterable


import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np


@dataclass
class Point:
    x: float
    y: float

    def distance(self, rhs: "Point") -> float:
        dx = self.x - rhs.x
        dy = self.y - rhs.y
        return math.sqrt(dx * dx + dy * dy)


class ErrorPlotter:
    def __init__(self, *csv_files: str) -> None:
        if len(csv_files) < 2:
            raise ValueError("must provide at least 2 CSV files")

        ref, *rest = csv_files

        self.__ref = ref
        self.__files = rest
        self.__ref_points = list(self.__read_csv(ref))
        self.__time = np.arange(len(self.__ref_points)) * 1e-4
        pass

    def plot(self, output: str) -> None:
        plt.xlabel("Time, s")
        plt.ylabel("Error")
        plt.title("ODE computation method comparison")
        plt.grid(True)

        self.plot_csv(self.__ref)
        for file in self.__files:
            self.plot_csv(file)

        plt.legend()
        plt.savefig(output, dpi=300)

    def plot_csv(self, file: str) -> None:
        dist = []
        for p1, p2 in zip(self.__ref_points, self.__read_csv(file)):
            dist.append(p1.distance(p2))
        plt.plot(self.__time, dist, label=file.removesuffix(".csv"))

    @staticmethod
    def __read_csv(file: str) -> Iterable[Point]:
        with open(file) as f:
            for line in f:
                _, _, x2, y2 = map(float, line.split(","))
                yield Point(x2, y2)


def main() -> None:
    csv_files = sys.argv[1:]
    plotter = ErrorPlotter(*csv_files)
    plotter.plot("methods.png")


if __name__ == "__main__":
    main()
