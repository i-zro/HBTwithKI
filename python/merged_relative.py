import csv
import numpy as np
import quaternion # pip install numpy numpy-quaternion numba
import numpy as np
from spatialmath import *
from spatialmath.pose3d import *
from math import pi

import matplotlib;

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

fields = []
rows = []

csv_data = []
krelative = []
irelative = []
compare = []

with open('merged.csv') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        csv_data.append(row)
    i_d = {val : i for i, val in enumerate(csv_data[0])}
    #print(i_d)
    """{'': 0, 'time': 1, 'imu_w': 2, 'imu_x': 3, 'imu_y': 4, 'imu_z': 5, 'neck': 6, 'neckx': 7, 'necky': 8, 'neckz': 9,
     'neckww': 10, 'neckwx': 11, 'neckwy': 12, 'neckwz': 13, 'neckconf': 14, 'ls': 15, 'lsx': 16, 'lsy': 17, 'lsz': 18,
     'lsww': 19, 'lswx': 20, 'lswy': 21, 'lswz': 22, 'lsconf': 23, 'le': 24, 'lex': 25, 'ley': 26, 'lez': 27,
     'leww': 28, 'lewx': 29, 'lewy': 30, 'lewz': 31, 'leconf': 32, 'lw': 33, 'lwx': 34, 'lwy': 35, 'lwz': 36,
     'lwww': 37, 'lwwx': 38, 'lwwy': 39, 'lwwz': 40, 'lwconf': 41}"""

def rel_func(a, b):
    ww0, wx0, wy0, wz0 = a
    ww1, wx1, wy1, wz1 = b
    q0 = np.quaternion(ww0, wx0, wy0, wz0)
    q1 = np.quaternion(ww1, wx1, wy1, wz1)
    relative_rot = quaternion.as_float_array(q1 * q0.inverse())
    relative_mat = [timestamp, relative_rot[0], relative_rot[1], relative_rot[2], relative_rot[3]]
    return relative_mat

relative = [] # 손목 relative 비교 (키넥트, imu 비교)

nrel = [] # 목
srel = [] # 어깨
erel = [] # 팔꿈치
ilist = []
klist = []
for i in range(1, len(csv_data)):
    timestamp = int(csv_data[i][i_d['time']])
    #print(timestamp)
    imu_data = list(map(float, csv_data[i][i_d['imu_w']:i_d['imu_z']+1]))
    ilist.append(imu_data)
    #print(imu_data)
    kin_data = list(map(float, csv_data[i][i_d['lwww']:i_d['lwwz']+1]))
    klist.append(kin_data)
    kneck = list(map(float, csv_data[i][i_d['neckww']:i_d['neckwz']+1]))
    kls = list(map(float, csv_data[i][i_d['lsww']:i_d['lswz'] + 1]))
    kle = list(map(float, csv_data[i][i_d['leww']:i_d['lewz']+1]))
    relative.append(rel_func(np.array(imu_data).astype(float), np.array(kin_data).astype(float)))
    nrel.append(rel_func(np.array(imu_data).astype(float), np.array(kneck).astype(float)))
    srel.append(rel_func(np.array(imu_data).astype(float), np.array(kls).astype(float)))
    erel.append(rel_func(np.array(imu_data).astype(float), np.array(kle).astype(float)))
print(relative)
print(nrel)
print(srel)
print(erel)

for i in range(1,len(csv_data)-1):
    timestamp = int(csv_data[i][1])
    imu_data = list(map(float, csv_data[i][i_d['imu_w']:i_d['imu_z'] + 1]))
    kin_data = list(map(float, csv_data[i][i_d['lwww']:i_d['lwwz'] + 1]))
    kin_data2 = list(map(float, csv_data[i + 1][i_d['imu_w']:i_d['imu_z']+1]))
    kin_data2 = list(map(float, csv_data[i + 1][i_d['lwww']:i_d['lwwz']+1]))
    # print(kin_data)
    ww0, wx0, wy0, wz0 = np.array(imu_data).astype(float)
    ww02, wx02, wy02, wz02 = np.array(imu_data).astype(float)
    ww1, wx1, wy1, wz1 = np.array(kin_data).astype(float)
    ww12, wx12, wy12, wz12 = np.array(kin_data2).astype(float)
    q0 = np.quaternion(ww0, wx0, wy0, wz0)
    q02 = np.quaternion(ww02, wx02, wy02, wz02)
    q1 = np.quaternion(ww1, wx1, wy1, wz1)
    q12 = np.quaternion(ww12, wx12, wy12, wz12)

    krel = quaternion.as_float_array(q02 * q0.inverse())
    irel = quaternion.as_float_array(q12 * q1.inverse())

    k_relmat = [timestamp, krel[0], krel[1], krel[2], krel[3]]
    i_relmat = [timestamp, irel[0], irel[1], irel[2], irel[3]]

    krelative.append(k_relmat) # kinect의 시간단위 rel
    irelative.append(i_relmat) # imu의 시간단위 rel

'''print(csv_data)
print(relative)
print(krelative)
print(irelative)
print(compare)'''

'''for i, j in zip(compare, krelative):
    print(i, j)'''

count = 0
w, h = plt.figaspect(0.5)
fig = plt.figure(figsize=(w, h),constrained_layout=True)
xx = fig.add_subplot(4, 4, 1, projection='3d')
xx2 = fig.add_subplot(4, 4, 2, projection='3d')
xx3 = fig.add_subplot(4, 4, 5, projection='3d')
xx4 = fig.add_subplot(4, 4, 6, projection='3d')
ax = fig.add_subplot(4, 4, 3, projection='3d')
ax2 = fig.add_subplot(4, 4, 4, projection='3d')
ax3 = fig.add_subplot(4, 4, 7, projection='3d')
ax4 = fig.add_subplot(4, 4, 8, projection='3d')
xx.set_xlim(-0.5, 0.5)
xx.set_ylim(-0.5, 0.5)
xx.set_zlim(0, 1)
xx2.set_xlim(-0.5, 0.5)
xx2.set_ylim(-0.5, 0.5)
xx2.set_zlim(0, 1)
xx3.set_xlim(-0.5, 0.5)
xx3.set_ylim(-0.5, 0.5)
xx3.set_zlim(0, 1)
xx4.set_xlim(-0.5, 0.5)
xx4.set_ylim(-0.5, 0.5)
xx4.set_zlim(0, 1)
ax.set_xlim(-0.5, 0.5)
ax.set_ylim(-0.5, 0.5)
ax.set_zlim(0, 1)
ax2.set_xlim(-0.5, 0.5)
ax2.set_ylim(-0.5, 0.5)
ax2.set_zlim(0, 1)
ax3.set_xlim(-0.5, 0.5)
ax3.set_ylim(-0.5, 0.5)
ax3.set_zlim(0, 1)
ax4.set_xlim(-0.5, 0.5)
ax4.set_ylim(-0.5, 0.5)
ax4.set_zlim(0, 1)
x1 = [xx.quiver(0, 0, 0, 0, 0, 0), xx.quiver(0, 0, 0, 0, 0, 0), xx.quiver(0, 0, 0, 0, 0, 0)]
x2 = [xx2.quiver(0, 0, 0, 0, 0, 0), xx2.quiver(0, 0, 0, 0, 0, 0), xx2.quiver(0, 0, 0, 0, 0, 0)]
x3 = [xx3.quiver(0, 0, 0, 0, 0, 0), xx3.quiver(0, 0, 0, 0, 0, 0), xx3.quiver(0, 0, 0, 0, 0, 0)]
x4 = [xx4.quiver(0, 0, 0, 0, 0, 0), xx4.quiver(0, 0, 0, 0, 0, 0), xx4.quiver(0, 0, 0, 0, 0, 0)]

a1 = [ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0), ax.quiver(0, 0, 0, 0, 0, 0)]
a2 = [ax2.quiver(0, 0, 0, 0, 0, 0), ax2.quiver(0, 0, 0, 0, 0, 0), ax2.quiver(0, 0, 0, 0, 0, 0)]
a3 = [ax3.quiver(0, 0, 0, 0, 0, 0), ax3.quiver(0, 0, 0, 0, 0, 0), ax3.quiver(0, 0, 0, 0, 0, 0)]
a4 = [ax4.quiver(0, 0, 0, 0, 0, 0), ax4.quiver(0, 0, 0, 0, 0, 0), ax4.quiver(0, 0, 0, 0, 0, 0)]

colors = ['#E16F6D', '#4eb604', '#0d87c3']

rows = compare.copy()

def update(count):
    r1 = ilist[count]
    qq1 = UnitQuaternion([float(r1[0]), float(r1[1]),float(r1[2]),float(r1[3])]).R
    for i in range(3):
        x1[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        x1[i] = xx.quiver(0, 0, 0.5, qq1[0][i], qq1[1][i], qq1[2][i],
                            length=0.5, normalize=True, color=colors[i])
        xx.set_title('lw_imu')
    r2 = klist[count]
    qq2 = UnitQuaternion([float(r2[0]), float(r2[1]),float(r2[2]),float(r2[3])]).R
    for i in range(3):
        x2[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        x2[i] = xx2.quiver(0, 0, 0.5, qq2[0][i], qq2[1][i], qq2[2][i],
                            length=0.5, normalize=True, color=colors[i])
        xx2.set_title('lw_kin')

    # relative
    if(count < len(irelative)-2):
        r3 = irelative[count]
        qq3 = UnitQuaternion([float(r3[1]), float(r3[2]), float(r3[3]), float(r3[4])]).R
        for i in range(3):
            x3[i].remove()
            # axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
            x3[i] = xx3.quiver(0, 0, 0.5, qq3[0][i], qq3[1][i], qq3[2][i],
                               length=0.5, normalize=True, color=colors[i])
            xx3.set_title('imu_rel')
        r4 = krelative[count]
        qq4 = UnitQuaternion([float(r4[1]), float(r4[2]), float(r4[3]), float(r4[4])]).R
        for i in range(3):
            x4[i].remove()
            # axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
            x4[i] = xx4.quiver(0, 0, 0.5, qq4[0][i], qq4[1][i], qq4[2][i],
                               length=0.5, normalize=True, color=colors[i])
            xx4.set_title('kin_rel')
    row=relative[count]
    q=UnitQuaternion([float(row[1]), float(row[2]),float(row[3]),float(row[4])]).R
    row2=nrel[count]
    q2=UnitQuaternion([float(row2[1]), float(row2[2]),float(row2[3]),float(row2[4])]).R
    row3 = srel[count]
    q3 = UnitQuaternion([float(row3[1]), float(row3[2]), float(row3[3]), float(row3[4])]).R
    row4 = erel[count]
    q4 = UnitQuaternion([float(row4[1]), float(row4[2]), float(row4[3]), float(row4[4])]).R
    for i in range(3):
        a1[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        a1[i] = ax.quiver(0, 0, 0.5, q[0][i], q[1][i], q[2][i],
                            length=0.5, normalize=True, color=colors[i])
        ax.set_title('lw and lw')
    for i in range(3):
        a2[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        a2[i] = ax2.quiver(0, 0, 0.5, q2[0][i], q2[1][i], q2[2][i],
                            length=0.5, normalize=True, color=colors[i])
        ax2.set_title('lw and neck')
    for i in range(3):
        a3[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        a3[i] = ax3.quiver(0, 0, 0.5, q3[0][i], q3[1][i], q3[2][i],
                            length=0.5, normalize=True, color=colors[i])
        ax3.set_title('lw and shoulder')
    for i in range(3):
        a4[i].remove()
        #axes[i] = ax.quiver(float(row[1])/1000, float(row[2])/1000, float(row[3])/1000, q[0][i], q[1][i], q[2][i], length=0.1, normalize=True, color=colors[i])
        a4[i] = ax4.quiver(0, 0, 0.5, q4[0][i], q4[1][i], q4[2][i],
                            length=0.5, normalize=True, color=colors[i])
        ax4.set_title('lw and shoulder')
ani = FuncAnimation(fig, update, frames=range(0, len(relative)), interval=50, repeat=False)
ani.save('relative.gif', writer='Pillow', fps=10)
#ani.save('com_rot.gif', writer='imagenick', fps=10)
plt.show()