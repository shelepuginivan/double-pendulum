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
    def frame_duration(self) -> float:
        return 1 / self.framerate

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

    def rod(self, dot1: Dot, dot2: Dot) -> Line:
        return (
            Line(dot1.get_center(), dot2.get_center())
            .set_color(GRAY)
            .add_updater(
                lambda mob: mob.become(
                    Line(dot1.get_center(), dot2.get_center()).set_color(GRAY)
                )
            )
        )

    def construct(self):
        rows = self.iter_rows()

        for row in rows:
            p1, p2 = row
            break
        else:
            raise RuntimeError("cannot determine initial coordinates")

        pivot = Dot().set_color(GRAY).set_z_index(1)
        dot1 = Dot().set_color(BLUE).move_to(p1).set_z_index(1)
        dot2 = Dot().set_color(RED).move_to(p2).set_z_index(1)
        rod1 = self.rod(pivot, dot1)
        rod2 = self.rod(dot1, dot2)
        self.add(pivot, dot1, dot2, rod1, rod2)
        self.wait()

        for p1, p2 in rows:
            self.play(
                dot1.animate.move_to(p1),
                dot2.animate.move_to(p2),
                run_time=self.cfg.frame_duration,
                rate_func=linear,
            )
