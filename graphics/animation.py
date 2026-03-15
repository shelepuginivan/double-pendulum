from typing import Iterable

from manim import *  # type: ignore
from manim.typing import Vector2D
from pydantic_settings import BaseSettings, SettingsConfigDict


class Config(BaseSettings):
    framerate: int
    dt: float
    dataset: str

    model_config = SettingsConfigDict(env_prefix="DP_GRAPHICS_")

    @property
    def rows_per_frame(self) -> int:
        frame_duration = 1 / self.framerate
        return int(frame_duration / self.dt)


class DoublePendulum(Scene):
    def __init__(self) -> None:
        cfg = Config()  # type: ignore
        self.cfg = cfg

        config.frame_rate = cfg.framerate

        super().__init__()

    def iter_rows(self) -> Iterable[tuple[Vector2D, Vector2D]]:
        with open(self.cfg.dataset) as csv:
            for idx, row in enumerate(csv):
                if idx % self.cfg.rows_per_frame != 0:
                    continue
                x1, y1, x2, y2 = map(float, row.split(","))
                yield x1 * RIGHT + y1 * UP, x2 * RIGHT + y2 * UP

    def construct(self):
        rows = self.iter_rows()

        for row in rows:
            p1, p2 = row
            break
        else:
            raise RuntimeError("cannot determine initial coordinates")

        pivot = Dot().set_color(RED)
        dot1 = Dot().move_to(p1)
        dot2 = Dot().move_to(p2)

        self.add(pivot, dot1, dot2)
        self.wait()
