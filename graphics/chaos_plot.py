import sys
from typing import Annotated, Any

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
from pydantic import Field, PositiveFloat, PositiveInt, field_validator
from pydantic_settings import BaseSettings, NoDecode


class Config(BaseSettings):
    dt: PositiveFloat = Field(default=0.0001, alias="dp_system_dt")
    framerate: PositiveInt = Field(default=2, alias="dp_chaos_framerate")
    colors: Annotated[list[str], NoDecode] = Field(
        default_factory=lambda: ["red", "blue", "green", "orange"],
        alias="dp_chaos_colors",
    )
    output: str = Field(default="chaos.png", alias="dp_chaos_output")

    @field_validator("colors", mode="before")
    @classmethod
    def validate_colors(cls, v: Any) -> list[str]:
        if isinstance(v, str):
            return v.split(",")
        else:
            return v

    @property
    def rows_per_frame(self) -> int:
        frame_duration = 1 / self.framerate
        return int(frame_duration / self.dt)


class Dataset:
    def __init__(self, csv_file: str) -> None:
        self.__csv_file = csv_file

    def plot(self, cfg: Config, color: str) -> None:
        with open(self.__csv_file) as csv:
            for idx, row in enumerate(csv):
                if idx % cfg.rows_per_frame != 0:
                    continue

                _, _, x2, y2 = map(float, row.split(","))
                plt.scatter(x2, y2, color=color, marker="o", s=1, alpha=0.5)


def main() -> None:
    cfg = Config()  # type: ignore
    csv_files = sys.argv[1:]

    plt.title("Double Pendulum Divergence")
    plt.xlabel("x")
    plt.ylabel("y")

    for csv, color in zip(csv_files, cfg.colors):
        Dataset(csv).plot(cfg, color)

    plt.savefig(cfg.output, dpi=300)


if __name__ == "__main__":
    main()
