from typing import Iterable

from manim import *  # type: ignore
from manim.typing import Vector2D
from pydantic import Field, PositiveFloat, PositiveInt
from pydantic_settings import BaseSettings


class Config(BaseSettings):
    dt: PositiveFloat = Field(default=0.0001, alias="dp_system_dt")
    framerate: PositiveInt = Field(default=24, alias="dp_animation_framerate")
    dataset: str = Field(alias="dp_animation_dataset")
    trail: float = Field(default=0.5, alias="dp_animation_trail")

    @property
    def frame_duration(self) -> float:
        return 1 / self.framerate

    @property
    def rows_per_frame(self) -> int:
        return int(self.frame_duration / self.dt)


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

        axes = NumberPlane().set_color(GRAY).set_opacity(0.1)
        pivot = Dot().set_color(GRAY).set_z_index(1)
        dot1 = Dot().set_color(BLUE).move_to(p1).set_z_index(1)
        dot2 = Dot().set_color(RED).move_to(p2).set_z_index(1)
        rod1 = self.rod(pivot, dot1)
        rod2 = self.rod(dot1, dot2)
        trail = TracedPath(
            dot2.get_center,
            dissipating_time=self.cfg.trail,
            stroke_color=RED,
        )
        self.add(axes, pivot, dot1, dot2, rod1, rod2, trail)
        self.wait()

        for p1, p2 in rows:
            self.play(
                dot1.animate.move_to(p1),
                dot2.animate.move_to(p2),
                run_time=self.cfg.frame_duration,
                rate_func=linear,
            )
