import math
import sys
from dataclasses import dataclass
from typing import Iterable

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
from pydantic import Field, PositiveFloat
from pydantic_settings import BaseSettings


class Config(BaseSettings):
    dt: PositiveFloat = Field(default=0.0001, alias="dp_system_dt")
    output: str = Field(default="methods.png", alias="dp_methods_output")


@dataclass
class Point:
    x: float
    y: float

    def distance(self, rhs: "Point") -> float:
        dx = self.x - rhs.x
        dy = self.y - rhs.y
        return math.sqrt(dx * dx + dy * dy)


class ErrorPlotter:
    def __init__(self, cfg: Config, *csv_files: str) -> None:
        if len(csv_files) < 2:
            raise ValueError("must provide at least 2 CSV files")

        ref, *rest = csv_files

        self.__ref = ref
        self.__files = rest
        self.__ref_points = list(self.__read_csv(ref))
        self.__time = np.arange(len(self.__ref_points)) * cfg.dt
        self.__output = cfg.output

    def plot(self) -> None:
        plt.xlabel("Time, s")
        plt.ylabel("Error")
        plt.title("ODE computation method comparison")
        plt.grid(True)

        self.plot_csv(self.__ref)
        for file in self.__files:
            self.plot_csv(file)

        plt.legend()
        plt.savefig(self.__output, dpi=300)

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
    cfg = Config()  # type: ignore
    csv_files = sys.argv[1:]
    plotter = ErrorPlotter(cfg, *csv_files)
    plotter.plot()


if __name__ == "__main__":
    main()
