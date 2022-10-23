#!/usr/bin/python3
import sys
from mpmath import *
from pi import pi
from e import e
MAGIC=18
partition_sizes = [ 10, 8 ]

def get_zero_digit(iter1, digit):
    mp.prec=64
    mp.dps=64
    zero = str(zetazero(iter1).imag)
    idx = zero.index(".")
    zero = zero[idx - 2:]
    return zero[digit]

def _partition_(n):
    ctr = 0
    ll = len(n)
    partitions = []
    t = 0
    while ctr == 0 or ctr % ll != 0:
        partition = "" 
        for x in range(0, partition_sizes[t]):
            partition = partition + n[ctr]
            ctr = ctr + 1
        t = 1 - t
        partitions.append(partition)
    return partitions

def coverage(set1, set2):
    set_of_numbers = set(sorted(set2))
    nhits = 0
    for x in set_of_numbers:
        m = set2.count(x)
        idx = -1
        while m > 0 and x in set1[idx + 1:]:
            idx = set1.index(x, idx + 1)
            nhits = nhits + 1
            m = m - 1
    return nhits

def analyze_helper1(posits):
    prev_pos = -1
    count = 0
    snippet = []
    for x in posits:
        if prev_pos > -1 and prev_pos == x - 1:
            snippet.append(str(bin(count)[2:][:-1]))
            prev_pos = x
            count = 1
            continue
        prev_pos = x
        count = count + 1
    if count > 0:
        snippet.append(str(bin(count)[2:]))
    return snippet


def analyze_helper2(_pp_, ee):
    counter = 0
    posits = []
    for x in list(zip(_pp_, ee)):
        if x[0] == x[1]:
            posits.append(counter + 1)
        counter = counter + 1
    f1 = analyze_helper1(posits)
    return f1

def analyze(_pp_, posits, index):
    f1 = analyze_helper1(posits)
    ee = e[:index][::-1]
    f2 = analyze_helper2(_pp_, ee)
    return "".join(f1), "".join(f2)

if __name__ == "__main__":
    num = str(sys.argv[1])
    num = num.lstrip().rstrip()
    n_iter = int(sys.argv[2])
    l = len(num)
    n = num*MAGIC
    ll = len(n)
    partitions = _partition_(n)
    _ctr = -1
    total_l = 0
    for partition in partitions:
        total_l = total_l + len(partition)
    iter1 =0
    interval = 0
    index = 0
    posits = []
    factor_higher_front_end = []
    factor_higher_rear_end = []
    while iter1 < n_iter:
        result = ""
        ctr = _ctr + 2
        f=open("./pi.txt","r")
        f.seek(ctr)
        g=open("./e.txt","r")
        g.seek(ctr)
        n_partition = str(f.read(total_l))
        r_partition = str(g.read(total_l))
        f.close()
        g.close()
        counter = 0
        prev_idx = 0
        for partition in partitions:
            set1 = "" 
            set2 = ""
            lp = len(partition)
            idx = 0
            while (idx+prev_idx) < prev_idx + lp:
                set1 = set1 + n_partition[idx + prev_idx]
                set2 = set2 + r_partition[idx + prev_idx]
                idx = idx + 1
            prev_idx = prev_idx + idx
            n_hits1 = coverage(set1, partition)
            n_hits2 = coverage(set2, partition)
            if len(result) == 0 and n_hits1 == n_hits2 and n_hits1 > 0:
                    result = get_zero_digit(iter1 + 1, counter)
            counter = counter + 1
        if len(result) == 0 or result == '0':
            pass
        elif result != '.':
            interval = interval + 1
        elif result == '.':
            output_file=str(sys.argv[3])
            h=open(output_file,"a")
            pp = pi[index]
            _pp_ = _pp_ + str(interval)
            if int(pp) == interval and interval == 0:
                posits.append(counter + 1)
                f1, f2 = analyze(_pp_, posits, index + 1)
                posits = []
                _pp_ = ""
                index = 0
            elif int(pp) == interval and interval > 0:
                posits.append(counter + 1)
            h.write(str(interval))
            h.close()
            interval = 0
            index = 0
        _ctr= _ctr + 2
        iter1 = iter1 + 1
