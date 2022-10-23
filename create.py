#!/usr/bin/python3
f=open("./zeros.hpp","w")
f.write("#ifndef _ZEROS_")
f.write("\n")
f.write("#define _ZEROS_")
f.write("\n")
f.write("int left_endings[8][10] = {")
g=open("./zeros1","r")
lines=g.readlines()
#left endings
for x in range(0, 7):
    f.write(" { ")
    for y in range(0, 10):
       f.write(" { ")
       for line in lines:
           l = str(line).lstrip().rstrip()
           if l[x] == str(y):
               f.write(str(l[x + 1]))
               f.write(",")
       f.write(" },")
    f.write(" },")
f.write(" };")
f.write("\n")
f.write("int right_endings[8][10] = {")
for x in range(1, 8):
    f.write(" { ")
    for y in range(0, 10):
       f.write(" { ")
       for line in lines:
           l = str(line).lstrip().rstrip()
           if l[x] == str(y):
               f.write(str(l[x - 1]))
               f.write(",")
       f.write(" },")
    f.write(" },")
f.write(" };")
f.write("\n")
f.write("#endif")
f.write("\n")
f.close()
g.close()
