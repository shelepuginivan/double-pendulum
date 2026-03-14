from manim import *  # type: ignore
from pydantic_settings import BaseSettings, SettingsConfigDict


class Config(BaseSettings):
    framerate: int
    dt: float

    model_config = SettingsConfigDict(env_prefix="DP_GRAPHICS_")


class DoublePendulum(Scene):
    def __init__(self) -> None:
        cfg = Config()  # type: ignore
        self.cfg = cfg

        config.frame_rate = cfg.framerate

        super().__init__()

    def construct(self):
        square = Square()
        self.play(Create(square))
        self.wait()
