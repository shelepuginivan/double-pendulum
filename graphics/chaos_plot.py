import sys
from dataclasses import dataclass

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
from pydantic_settings import BaseSettings, SettingsConfigDict


class Config(BaseSettings):
    framerate: int
    dt: float
    colors: list[str] = ["red", "blue", "green", "orange", "purple", "brown"]
    plot_output: str = "chaos.png"

    model_config = SettingsConfigDict(env_prefix="DP_GRAPHICS_")


@dataclass
class Dataset:
    csv_file: str

    def plot(self, cfg: Config, color: str) -> None:
        frame_duration = 1 / cfg.framerate
        rows_per_frame = int(frame_duration / cfg.dt)

        with open(self.csv_file) as csv:
            for idx, row in enumerate(csv):
                if idx % rows_per_frame != 0:
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

    plt.savefig(cfg.plot_output, dpi=300)


if __name__ == "__main__":
    main()
