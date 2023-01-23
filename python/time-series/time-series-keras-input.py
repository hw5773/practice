import random
import numpy as np
import matplotlib.pyplot as plt

import pickle as pkl
import keras
from keras.models import Sequential, Model, load_model
from keras.layers import LSTM, Dense, RepeatVector, TimeDistributed, Input, BatchNormalization, multiply, concatenate, Flatten, Activation, dot
from keras.callbacks import EarlyStopping
import pydot as pyd
from keras.utils.vis_utils import plot_model, model_to_dot
keras.utils.vis_utils.pydot = pyd

def truncate(x, feature_cols=range(3), target_cols=range(3), label_col=3, train_len=100, test_len=20):
    in_, out_, lbl = [], [], []
    for i in range(len(x)-train_len-test_len+1):
        in_.append(x[i:(i+train_len), feature_cols].tolist())
        out_.append(x[(i+train_len):(i+train_len+test_len), target_cols].tolist())
        lbl.append(x[i+train_len, label_col])
    return np.array(in_), np.array(out_), np.array(lbl)

n_ = 1000
t = np.linspace(0, 50*np.pi, n_)

x1 = sum([20*np.sin(i*t+np.pi) for i in range(5)]) + 0.01*(t**2) + np.random.normal(0, 6, n_)
x2 = sum([15*np.sin(2*i*t+np.pi) for i in range(5)]) + 0.5*t + np.random.normal(0, 6, n_)

train_ratio = 0.8
train_len = int(train_ratio * t.shape[0])
print ("train_len: {}".format(train_len))

x_index = np.array(range(len(t)))
x1_trend_param = np.polyfit(x_index[:train_len], x1[:train_len], 2)
x2_trend_param = np.polyfit(x_index[:train_len], x2[:train_len], 1)

print ("x1_trend_param: {}".format(x1_trend_param))
print ("x2_trend_param: {}".format(x2_trend_param))

x1_trend = (x_index ** 2) * x1_trend_param[0] + x_index * x1_trend_param[1] + x1_trend_param[2]
x2_trend = x_index * x2_trend_param[0] + x2_trend_param[1]

x1_detrend = x1 - x1_trend
x2_detrend = x2 - x2_trend

#print ("x1_detrend: {}".format(x1_detrend))
#print ("x2_detrend: {}".format(x2_detrend))

x_lbl = np.column_stack([x1_detrend, x2_detrend, x_index, [1]*train_len+[0]*(len(x_index)-train_len)])
print ("x_lbl.shape: {}".format(x_lbl.shape))
print ("x_lbl: {}".format(x_lbl))

x_train_max = x_lbl[x_lbl[:, 3]==1, :2].max(axis=0)
print ("x_train_max 1: {}".format(x_train_max))
x_train_max = x_train_max.tolist() + [1]*2
print ("x_train_max 2: {}".format(x_train_max))

x_normalize = np.divide(x_lbl, x_train_max)
print ("x_normalize: {}".format(x_normalize))

"""
plt.figure(figsize=(15, 4))
#plt.plot(range(len(x1)), x1, label='x1')
#plt.plot(range(len(x1_trend)), x1_trend, linestyle='--', label='x1_trend')
#plt.plot(range(len(x2)), x2, label='x2')
#plt.plot(range(len(x2_trend)), x2_trend, linestyle='--', label='x2_trend')
plt.plot(range(len(x1_detrend)), x1_detrend, label='x1_detrend')
plt.plot(range(len(x2_detrend)), x2_detrend, label='x2_detrend')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=2)
plt.show()

plt.figure(figsize=(15, 4))
plt.plot(range(train_len), x_normalize[:train_len, 0], label='x1_train_normalized')
plt.plot(range(train_len), x_normalize[:train_len, 1], label='x2_train_normalized')
plt.plot(range(train_len, len(x_normalize)), x_normalize[train_len:, 0], label='x1_test_normalized')
plt.plot(range(train_len, len(x_normalize)), x_normalize[train_len:, 1], label='x1_test_normalized')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=2)
plt.show()
"""

# Making three sets including the input set, the output set, and the label set (the index binds each entry)
X_in, X_out, lbl = truncate(x_normalize, feature_cols=range(3), target_cols=range(3), label_col=3, train_len=200, test_len=20)
print("X_in.shape: {}, X_out.shape:{}, lbl.shape: {}".format(X_in.shape, X_out.shape, lbl.shape))

X_input_train = X_in[np.where(lbl==1)]
X_output_train = X_out[np.where(lbl==1)]
X_input_test = X_in[np.where(lbl==0)]
X_output_test = X_out[np.where(lbl==0)]
print("X_input_train.shape: {}, X_output_train.shape: {}".format(X_input_train.shape, X_output_train.shape))
print("X_input_test.shape: {}, X_output_test.shape: {}".format(X_input_test.shape, X_output_test.shape))

input_train = Input(shape=(X_input_train.shape[1], X_input_train.shape[2]-1))
output_train = Input(shape=(X_output_train.shape[1], X_output_train.shape[2]-1))
print ("input_train: {}".format(input_train))
print ("output_train: {}".format(output_train))
