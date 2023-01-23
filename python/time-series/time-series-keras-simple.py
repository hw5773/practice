import random
import numpy as np
import matplotlib.pyplot as plt

from tensorflow.keras.optimizers import Adam
import pickle as pkl
import keras
from keras.models import Sequential, Model, load_model
from keras.layers import LSTM, Dense, RepeatVector, TimeDistributed, Input, BatchNormalization, multiply, concatenate, Flatten, Activation, dot
from keras.callbacks import EarlyStopping
import pydot as pyd
from tensorflow.keras.utils import plot_model
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

n_hidden = 100

input_train = Input(shape=(X_input_train.shape[1], X_input_train.shape[2]-1))
output_train = Input(shape=(X_output_train.shape[1], X_output_train.shape[2]-1))
print ("input_train: {}".format(input_train))
print ("output_train: {}".format(output_train))

encoder_last_h1, encoder_last_h2, encoder_last_c = LSTM(n_hidden, activation='elu', dropout=0.2, recurrent_dropout=0.2, return_sequences=False, return_state=True)(input_train)
print ("encoder_last_h1: {}".format(encoder_last_h1))
print ("encoder_last_h2: {}".format(encoder_last_h2))
print ("encoder_last_c: {}".format(encoder_last_c))

encoder_last_h1 = BatchNormalization(momentum=0.6)(encoder_last_h1)
encoder_last_c = BatchNormalization(momentum=0.6)(encoder_last_c)

decoder = RepeatVector(output_train.shape[1])(encoder_last_h1)
decoder = LSTM(n_hidden, activation='elu', dropout=0.2, recurrent_dropout=0.2, return_state=False, return_sequences=True)(decoder, initial_state=[encoder_last_h1, encoder_last_c])
print ("decoder: {}".format(decoder))

out = TimeDistributed(Dense(output_train.shape[2]))(decoder)
print (out)

model = Model(inputs=input_train, outputs=out)
opt = Adam(learning_rate=0.01, clipnorm=1)
model.compile(loss='mean_squared_error', optimizer=opt, metrics=['mae'])
model.summary()

#plot_model(model, to_file='model_plot.png', show_shapes=True, show_layer_names=True)

epc = 5
es = EarlyStopping(monitor='val_loss', mode='min', patience=50)
history = model.fit(X_input_train[:, :, :2], X_output_train[:, :, :2], validation_split=0.2, epochs=epc, verbose=1, callbacks=[es], batch_size=100)
train_mae = history.history['mae']
valid_mae = history.history['val_mae']

model.save('model_forecasting_simple.h5')

"""
plt.plot(train_mae, label='train_mae')
plt.plot(valid_mae, label='validation_mae')
plt.ylabel('mae')
plt.xlabel('epoch')
plt.title('train v.s. validation accuracy (mae)')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=2)
plt.show()
"""

train_pred_detrend = model.predict(X_input_train[:, :, :2]) * x_train_max[:2]
test_pred_detrend = model.predict(X_input_test[:, :, :2]) * x_train_max[:2]
print ("train_pred_detrend.shape: {}".format(train_pred_detrend.shape))
print ("test_pred_detrend.shape: {}".format(test_pred_detrend.shape))

train_true_detrend = X_output_train[:, :, :2] * x_train_max[:2]
test_true_detrend = X_output_test[:, :, :2] * x_train_max[:2]
print ("train_true_detrend.shape: {}".format(train_true_detrend.shape))
print ("test_true_detrend.shape: {}".format(test_true_detrend.shape))

train_pred_detrend = np.concatenate([train_pred_detrend, np.expand_dims(X_output_train[:, :, 2], axis=2)], axis=2)
test_pred_detrend = np.concatenate([test_pred_detrend, np.expand_dims(X_output_test[:, :, 2], axis=2)], axis=2)
print ("train_pred_detrend.shape: {}".format(train_pred_detrend.shape))
print ("test_pred_detrend.shape: {}".format(test_pred_detrend.shape))

train_true_detrend = np.concatenate([train_true_detrend, np.expand_dims(X_output_train[:, :, 2], axis=2)], axis=2)
test_true_detrend = np.concatenate([test_true_detrend, np.expand_dims(X_output_test[:, :, 2], axis=2)], axis=2)
print ("train_true_detrend.shape: {}".format(train_true_detrend.shape))
print ("test_true_detrend.shape: {}".format(test_true_detrend.shape))

data_final = dict()
for dt, lb in zip([train_pred_detrend, train_true_detrend, test_pred_detrend, test_true_detrend], ['train_pred', 'train_true', 'test_pred', 'test_true']):
    dt_x1 = dt[:, :, 0] + (dt[:, :, 2]**2)*x1_trend_param[0] + dt[:, :, 2]*x1_trend_param[1] + x1_trend_param[2]
    dt_x2 = dt[:, :, 1] + dt[:, :, 2]*x2_trend_param[0] + x2_trend_param[1]
    data_final[lb] = np.concatenate([np.expand_dims(dt_x1, axis=2), np.expand_dims(dt_x2, axis=2)], axis=2)
    print(lb + ": {}".format(data_final[lb].shape))

for lb in ['train', 'test']:
    plt.figure(figsize=(15, 4))
    plt.hist(data_final[lb+'_pred'].flatten(), bins=100, color='orange', alpha=0.5, label=lb + ' pred')
    plt.hist(data_final[lb+'_true'].flatten(), bins=100, color='green', alpha=0.5, label=lb + ' true')
    plt.legend()
    plt.title('value distribution: {}'.format(lb))
    plt.show()

for lb in ['train', 'test']:
    MAE_overall = abs(data_final[lb + '_pred'] - data_final[lb + '_true']).mean()
    MAE_ = abs(data_final[lb + '_pred'] - data_final[lb + '_true']).mean(axis=(1, 2))
    plt.figure(figsize=(15, 3))
    plt.plot(MAE_)
    plt.title("MAE " + lb + ": overall MAE = " + str(MAE_overall))
    plt.show()

ith_timestep = random.choice(range(data_final[lb + "_pred"].shape[1]))
plt.figure(figsize=(15, 5))
train_start_t = 0
test_start_t = data_final['train_pred'].shape[0]
for lb, tm, clrs in zip(["train", "test"], [train_start_t, test_start_t], [['green', 'red'], ['blue', 'orange']]):
    for i, x_lbl in zip([0, 1], ['x1', 'x2']):
        plt.plot(range(tm, tm + data_final[lb + "_pred"].shape[0]),
                data_final[lb + "_pred"][:, ith_timestep, i],
                linestyle='--', linewidth=1, color=clrs[0], label='pred ' + x_lbl)
        plt.plot(range(tm, tm + data_final[lb + '_pred'].shape[0]),
                data_final[lb + '_true'][:, ith_timestep, i],
                linestyle='--', linewidth=1, color=clrs[1], label='true ' + x_lbl)

plt.title("{}th time step in all samples".format(ith_timestep))
plt.legend(loc="upper center", bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=8)
plt.show()

lb = 'test'
plt.figure(figsize=(15, 5))
for i, x_lbl, clr in zip([0, 1], ['x1', 'x2'], ['green', 'blue']):
    plt.plot(data_final[lb + "_pred"][:, ith_timestep, i],
            linestyle='--', color=clr, label='pred ' + x_lbl)
    plt.plot(data_final[lb + "_true"][:, ith_timestep, i],
            linestyle='--', color=clr, label='true ' + x_lbl)
plt.title("({}): {}th time step in all samples".format(lb, ith_timestep))
plt.legend(loc="upper center", bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=2)
plt.show()
