import sys

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt


def plot_csv(csv_file: str, color: str) -> None:
    with open(csv_file) as f:
        for i, line in enumerate(f):
            if i % 5000 != 0:
                continue

            _, _, x2, y2 = map(float, line.split(","))
            plt.scatter(x2, y2, marker="o", c=color, s=1)


def main() -> None:
    csv_files = sys.argv[1:]

    plt.title("Double Pendulum Divergence")

    for csv in csv_files:
        plot_csv(csv, "red")

    plt.savefig("chaos.png")


if __name__ == "__main__":
    main()
