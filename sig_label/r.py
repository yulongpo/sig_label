# -*- coding: utf-8 -*-
"""
Created on Fri Jun 29 14:43:50 2018

@author: huanghao
"""

import numpy as np
import os
import matplotlib.pyplot as plt

f = open("test.data", "rb")

f.seek(0, os.SEEK_END)
print(f.tell())
f.seek(0)

l = list(np.frombuffer(f.read(4), np.int32))[0]
s = [int(i) for i in np.frombuffer(f.read(l), np.int8)]
k = list(np.frombuffer(f.read(4), np.int32))[0]

label = []
for _ in range(k):
    _b = [int(i) for i in np.frombuffer(f.read(8), np.uint32)]
    label.append(_b)
    
#e = list(np.frombuffer(f.read(4), np.int32))
#x = [int(i) for i in np.frombuffer(f.read(), np.int8)]

f.close()

xl = list(range(l))

plt.figure()
plt.plot(xl, s)
for st, en in label:
    plt.plot(xl[st:en], s[st:en], 'r')
