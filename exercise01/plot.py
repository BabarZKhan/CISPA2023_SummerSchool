#! /usr/bin/env python3


import matplotlib.pyplot as plt
import sys


def parse_data(fname):
    div = []
    xor = []
    with open(fname) as fd:
        for line in fd:
            hit_miss, timing = line.split(";")
            if hit_miss == "div":
                if int(timing) > 1000:
                    continue
                div.append(int(timing))
                pass
            elif hit_miss == "xor":
                if int(timing) > 1000:
                    continue
                xor.append(int(timing))
            else:
                print("Ill-formatted CSV file!")
                assert False
    return div, xor


def main():
    if len(sys.argv) == 2:
        fname = sys.argv[1]
    else:
        fname = "timings.log"

    div, xor = parse_data(fname)

    plt.hist(div, bins=range(0, 400, 2), color="orange",label="div")
    plt.hist(xor, bins=range(0, 400, 2), color="blue",label="xor")
    plt.legend()
    plt.savefig("working_solution.png")
    plt.show()


if __name__ == "__main__":
    main()
