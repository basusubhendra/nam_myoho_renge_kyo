#!/usr/bin/python3
import sys

def characterize(num):
    tuples = []
    l = len(num)
    ctr = 0
    while (1):
        tuples.append(num[ctr] + num[ctr + 1])
        ctr = ctr + 1
        if ctr == (l - 1):
            break
    return tuples

if __name__ == "__main__":
    num = str(sys.argv[1])
    tuples = characterize(num)
    print(tuples)
