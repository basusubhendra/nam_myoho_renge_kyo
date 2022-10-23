#!/usr/bin/python3

import sys
<<<<<<< HEAD
from flask import *

def factorize(num):
    num = str(num)
    return num

app = Flask(__name__)

@app.route('/')
def index():
    return "Usage: /num/<number to be factored>"

@app.get('/num/<number>')
def _factorize_get_(number):
    num = str(number)
    return factorize(num)

@app.post('/num')
def _factorize_post_():
    num = str(request.form['number'])
    return factorize(num)
=======
import redis
from zeros import *

def satisfies(x, y):
    x = int(x)
    y = int(y)
    if x == 0 or y == 0:
        return False
    if x == y:
        return False
    odd_X = x % 2
    odd_Y = y % 2
    if odd_X*odd_Y == 0 and odd_X != odd_Y:
        return True
    else:
        return False
    return False

if __name__ == "__main__":
    num = str(sys.argv[1])
    l = len(num)
    server = redis.Redis(host='localhost',port='6379',db=0)
    counter = 0
    prev_pos = -1
    cnt = 0
    _cnt = 0
    while True:
        pos = 1
        while True:
            _cc = []
            _dd = []
            hash_map = dict([])
            f = open("./pi.txt","r")
            g = open("./e.txt","r")
            ctr = 0
            while ctr < pos:
                c = str(f.read(12))
                d = str(g.read(12))
                _cc.append(c)
                _dd.append(d)
                ctr = ctr + 1
            __dd = []
            _dd = _dd[::-1]
            for x in _dd:
                __dd.append(x[::-1])
            _dd = __dd
            m = 0
            cnt = _cnt
            for zz in list(zip(_cc, _dd)):
                for xx in list(zip(zz[0], zz[1])):
                    if xx[0] == num[cnt % l] or xx[1] == num[cnt % l]:
                        if satisfies(xx[0], xx[1]) == True:
                            cnt  = cnt + 1
                            if m >= 2:
                                counter = counter + 1
                    m = m + 1
            hash_map[cnt % l] = counter
            print([counter, cnt % l, num[cnt % l]])
            y = str(input("?")) 
            if y == 'y':
               counter = 0
               prev_pos = cnt % l
               _cnt = cnt
               break
            pos = pos + 1
            counter = 0
        f.close()
        g.close()


>>>>>>> cca2728 (First Commit)
