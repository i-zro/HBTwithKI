import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as p3
from mpl_toolkits.mplot3d import proj3d
import matplotlib.animation as animation
import csv
import pandas as pd
import numpy as np
import quaternion # pip install numpy numpy-quaternion numba
import numpy as np
from spatialmath import *
from spatialmath.pose3d import *
from math import pi
import matplotlib;

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
plt.rc('font', family='Malgun Gothic')
plt.rcParams['axes.unicode_minus'] = False #한글 폰트 사용시 마이너스 폰트 깨짐 해결


import os, glob
import os.path

# kinect csv 불러와서 데이터프레임으로
col_names = ['t', 'neck', 'neckx', 'necky', 'neckz',
                                            'neckww', 'neckwx', 'neckwy', 'neckwz', 'neckconf',
                                            'ls', 'lsx', 'lsy', 'lsz',
                                            'lsww', 'lswx', 'lswy', 'lswz', 'lsconf',
                                            'le', 'lex', 'ley', 'lez',
                                            'leww', 'lewx', 'lewy', 'lewz', 'leconf',
                                            'lw', 'lwx', 'lwy', 'lwz',
                                            'lwww', 'lwwx', 'lwwy', 'lwwz', 'lwconf'
                                            ]
def k_to_df():
    k_csv = 'K*.csv'
    kinfile = glob.glob(k_csv)
    kin_df = pd.read_csv(kinfile[0], names = col_names)
    kin_df.set_index('t', inplace = True) # 시간을 인덱스로
    return kin_df

def kin_to_df():
    k_csv = 'K*.csv'
    kinfile = glob.glob(k_csv)
    kin_df = pd.read_csv(kinfile[0], names = col_names)
    return kin_df


class Simulator:

    def __init__(self, neck_pos, ls_pos, le_pos, lw_pos,
                 x_min, x_max, y_min, y_max, z_min, z_max,
                 neck_ori, ls_ori, le_ori, lw_ori, line):
        s_1 = neck_pos
        s_2 = ls_pos
        s_3 = le_pos
        s_4 = lw_pos

        o_n = neck_ori
        s_n = ls_ori
        e_n = le_ori
        w_n = lw_ori

        self.data = {
        '목': {'raw': s_1, 'ori': o_n},
        '왼쪽\n어깨': {'raw': s_2, 'ori': s_n},
        '왼쪽\n팔꿈치': {'raw': s_3, 'ori': e_n},
        '왼쪽\n손목': {'raw': s_4, 'ori': w_n},
        }

        # 축 설정
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')

        self.ax.set_xlim3d([x_min-100, x_max+100])
        self.ax.set_xlabel('X')

        self.ax.set_ylim3d([y_min-100, y_max+100])
        self.ax.set_ylabel('Y')

        self.ax.set_zlim3d([z_min-100, z_max+100])
        self.ax.set_zlabel('Z')
        self.line = line
        self.axes = [self.ax.quiver(0, 0, 0, 0, 0, 0), self.ax.quiver(0, 0, 0, 0, 0, 0), self.ax.quiver(0, 0, 0, 0, 0, 0)]
        self.lines = [self.ax.plot([], [], [], "b")]
        self.a2 = [self.ax.quiver(0, 0, 0, 0, 0, 0), self.ax.quiver(0, 0, 0, 0, 0, 0),
                     self.ax.quiver(0, 0, 0, 0, 0, 0)]
        self.a3 = [self.ax.quiver(0, 0, 0, 0, 0, 0), self.ax.quiver(0, 0, 0, 0, 0, 0),
                     self.ax.quiver(0, 0, 0, 0, 0, 0)]
        self.a4 = [self.ax.quiver(0, 0, 0, 0, 0, 0), self.ax.quiver(0, 0, 0, 0, 0, 0),
                     self.ax.quiver(0, 0, 0, 0, 0, 0)]
        self.colors = ['#E16F6D', '#4eb604', '#0d87c3']  # RGB 색 hex 값

        for point,dic in self.data.items():
            dic['x'] = []
            dic['y'] = []
            dic['z'] = []
            dic['ww'] = []
            dic['wx'] = []
            dic['wy'] = []
            dic['wz'] = []
            dic['length'] = len(dic['raw'])
            for coords in dic['raw']:
                dic['x'].append(coords[0])
                dic['y'].append(coords[1])
                dic['z'].append(coords[2])

            print(self.data.items())
            for cords in dic['ori']:
                dic['ww'].append(cords[0])
                dic['wx'].append(cords[1])
                dic['wy'].append(cords[2])
                dic['wz'].append(cords[3])


        # 애니메이션
        self.anim = animation.FuncAnimation(self.fig, self.update, init_func=self.setup, interval=1000)
        #self.anim.save('plot_joints.gif', writer='imagemagick')
        plt.show()


    def setup(self):
        plots = []

        for point,dic in self.data.items():
            dic['plot'] = self.ax.scatter3D([], [], [], c='#e377c2', picker = True)
            dic['label'] = self.ax.text2D(dic['x'][0], dic['y'][0], point, zorder=1, color='k')
    def update(self, i):

        plots = []

        seq_x = []
        seq_y = []
        seq_z = []

        seg = []

        for point,dic in self.data.items():
            if i < dic['length']:
                #self.lines[0] = self.ax.plot(self.line[i], self.line[i + 1], self.line[i + 2], '--')
                seq_x = dic['x'][i]
                seq_y = dic['y'][i]
                seq_z = dic['z'][i]
                dic['plot']._offsets3d = [seq_x], [seq_y], [seq_z]

                ori_w = dic['ww'][i]
                ori_x = dic['wx'][i]
                ori_y = dic['wy'][i]
                ori_z = dic['wz'][i]

                q = UnitQuaternion([ori_w, ori_x, ori_y, ori_z]).R
                if point =='목':
                    for i in range(3):
                        self.axes[i].remove()
                        self.axes[i] = self.ax.quiver(seq_x, seq_y, seq_z, q[0][i], q[1][i], q[2][i],
                                            length=50, normalize=True, color=self.colors[i])
                elif point == '왼쪽\n어깨':
                    for i in range(3):
                        self.a2[i].remove()
                        self.a2[i] = self.ax.quiver(seq_x, seq_y, seq_z, q[0][i], q[1][i], q[2][i],
                                                      length=50, normalize=True, color=self.colors[i])
                elif point == '왼쪽\n팔꿈치':
                    for i in range(3):
                        self.a3[i].remove()
                        self.a3[i] = self.ax.quiver(seq_x, seq_y, seq_z, q[0][i], q[1][i], q[2][i],
                                                    length=50, normalize=True, color=self.colors[i])

                elif point == '왼쪽\n손목':
                    for i in range(3):
                        self.a4[i].remove()
                        self.a4[i] = self.ax.quiver(seq_x, seq_y, seq_z, q[0][i], q[1][i], q[2][i],
                                                      length=50, normalize=True, color=self.colors[i])
                #### 글자 위치
                x2, y2, _ = proj3d.proj_transform(seq_x, seq_y, seq_z, self.ax.get_proj())
                dic['label'].set_position((x2,y2))

            else:
                self.anim.event_source.stop()

        return plots

def xyz_min_max(df, x_pos, y_pos, z_pos):
    x_min = df.iloc[:, x_pos].to_numpy().min()
    x_max = df.iloc[:, x_pos].to_numpy().max()
    y_min = df.iloc[:, y_pos].to_numpy().min()
    y_max = df.iloc[:, y_pos].to_numpy().max()
    z_min = df.iloc[:, z_pos].to_numpy().min()
    z_max = df.iloc[:, z_pos].to_numpy().max()

    return x_min, x_max, y_min, y_max, z_min, z_max
kin_df = k_to_df()
def df_to_array(df): # 데이터 프레임과 열번호가 담긴 리스트
    return_array = [] # 반환할 리스트
    for i in range(len(df.columns)):
        return_array.append(list(np.array(df.iloc[:, i].tolist())))

    return return_array
all_kin = [i for i in range(len(col_names)-1)]
tpos = [i for i in all_kin if i%9 == 1 or i%9 == 2 or i%9 == 3] # 시간 + 위치
tpos_kin_df = kin_df.iloc[:, tpos] # 시간 + 위치
x_pos = [i for i in range(len(tpos_kin_df.columns)) if i%3 == 0]
y_pos = [i for i in range(len(tpos_kin_df.columns)) if i%3 == 1]
z_pos = [i for i in range(len(tpos_kin_df.columns)) if i%3 == 2]
x_min, x_max, y_min, y_max, z_min, z_max \
    = xyz_min_max(tpos_kin_df, x_pos, y_pos, z_pos)
basic_df = kin_to_df()
print(basic_df.iloc[:, 1:10])
neck = [[rows[0], rows[1], rows[2], rows[3], rows[4], rows[5], rows[6], rows[7], rows[8]] for idx, rows in basic_df.iloc[:, 1:10].iterrows()]
neck_pos = [[rows[1], rows[2], rows[3]] for idx, rows in basic_df.iloc[:, 1:10].iterrows()]
neck_ori = [[rows[4], rows[5], rows[6], rows[7]] for idx, rows in basic_df.iloc[:, 1:10].iterrows()]
ls = [[rows[0], rows[1], rows[2], rows[3], rows[4], rows[5], rows[6], rows[7], rows[8]] for idx, rows in basic_df.iloc[:, 10:19].iterrows()]
ls_pos = [[rows[1], rows[2], rows[3]] for idx, rows in basic_df.iloc[:, 10:19].iterrows()]
ls_ori = [[rows[4], rows[5], rows[6], rows[7]] for idx, rows in basic_df.iloc[:, 10:19].iterrows()]
le_pos = [[rows[1], rows[2], rows[3]] for idx, rows in basic_df.iloc[:, 19:28].iterrows()]
le_ori = [[rows[4], rows[5], rows[6], rows[7]] for idx, rows in basic_df.iloc[:, 19:28].iterrows()]
lw_pos = [[rows[1], rows[2], rows[3]] for idx, rows in basic_df.iloc[:, 28:37].iterrows()]
lw_ori = [[rows[4], rows[5], rows[6], rows[7]] for idx, rows in basic_df.iloc[:, 28:37].iterrows()]
line = [[n[i], s[i], e[i], w[i]] for n, s, e, w in zip(neck_pos, ls_pos, le_pos, lw_pos) for i in range(3)]
print(line)
print(neck)
Simulator(neck_pos, ls_pos, le_pos, lw_pos, x_min, x_max, y_min, y_max, z_min, z_max, neck_ori, ls_ori, le_ori, lw_ori, line)
