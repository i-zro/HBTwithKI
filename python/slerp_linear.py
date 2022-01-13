import pandas as pd
import numpy as np
from scipy.spatial.transform import Slerp
from scipy.spatial.transform import Rotation as R
import os, glob
import os.path

# 파일 이름
k_csv = 'K*.csv'
i_csv = 'I*.csv'
kinfile = glob.glob(k_csv)
ifile = glob.glob(i_csv)

def interp(new_freq = 10, method = "slerp"):
    current_df = pd.read_csv(ifile[0], sep=",", names=['time', 'id', 'x', 'y', 'z', 'w'])
    current_df.drop_duplicates()
    kin_df = pd.read_csv(kinfile[0], sep=",", names=['time', 'j', 'px', 'py', 'pz', 'w', 'x', 'y', 'z', 'con'])
    old_times = np.array(current_df['time'])
    kin_times = np.array(kin_df['time'])
    #print(current_df)
    #print(kin_df)
    new_times = np.concatenate((old_times, kin_times), axis=0)
    np.sort(new_times)
    print(new_times)
    o_x = np.array(current_df['x'])
    o_y = np.array(current_df['y'])
    o_z = np.array(current_df['z'])
    o_w = np.array(current_df['w'])
    quats = []
    for t in range(len(old_times)):
        quats.append([o_x[t], o_y[t], o_z[t], o_w[t]])

    quats_object = R.from_quat(quats)

    slerp = Slerp(np.array(current_df['time']), quats_object)
    interp_rots = slerp(new_times)
    new_quats = interp_rots.as_quat()

    # Create new orientation objects
    ori_x = np.array([item[0] for item in new_quats])
    ori_y = np.array([item[1] for item in new_quats])
    ori_z = np.array([item[2] for item in new_quats])
    ori_w = np.array([item[3] for item in new_quats])
    print(o_x)


def interp_df(new_freq = 10, method = "linear"):
    current_df = pd.read_csv(ifile[0], sep=",", names=['time', 'id', 'imu_w','imu_x', 'imu_y','imu_z'])
    current_df.drop_duplicates()
    old_times = np.array(current_df['time'])
    new_df = pd.DataFrame()
    new_times = np.arange(min(current_df['time']), max(current_df['time']))
    kin_df = pd.read_csv(kinfile[0], sep=",", names = ['time', 'neck', 'neckx', 'necky', 'neckz',
                                            'neckww', 'neckwx', 'neckwy', 'neckwz', 'neckconf',
                                            'ls', 'lsx', 'lsy', 'lsz',
                                            'lsww', 'lswx', 'lswy', 'lswz', 'lsconf',
                                            'le', 'lex', 'ley', 'lez',
                                            'leww', 'lewx', 'lewy', 'lewz', 'leconf',
                                            'lw', 'lwx', 'lwy', 'lwz',
                                            'lwww', 'lwwx', 'lwwy', 'lwwz', 'lwconf'
                                            ])
    kin_times = np.array(kin_df['time'])
    kin_min = min(kin_times) # kinect 받은 값 중 최소

    #print(kin_times)
    o_x = np.array(current_df['imu_x'])
    o_y = np.array(current_df['imu_y'])
    o_z = np.array(current_df['imu_z'])
    o_w = np.array(current_df['imu_w'])

    if method == "linear":
        ori_x = np.interp(new_times, old_times, o_x)
        ori_y = np.interp(new_times, old_times, o_y)
        ori_z = np.interp(new_times, old_times, o_z)
        ori_w = np.interp(new_times, old_times, o_w)

    new_df['time'] = pd.Series(new_times)
    new_df['imu_w'] = pd.Series(ori_w)
    new_df['imu_x'] = pd.Series(ori_x)
    new_df['imu_y'] = pd.Series(ori_y)
    new_df['imu_z'] = pd.Series(ori_z)


    merge_outer = pd.merge(new_df, kin_df, how='inner', on='time')
    merge_outer.to_csv("merged.csv", mode="w")
    #print(merge_outer)


interp_df()
