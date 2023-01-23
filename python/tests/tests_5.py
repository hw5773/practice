import numpy as np
import matplotlib.pyplot as plt

def truncate(x, feature_cols=range(3), target_cols=range(3), label_col=3, train_len=5, test_len=2):
    in_, out_, lbl = [], [], []
    print ("len(x): {}".format(len(x)))
    print ("train_len: {}".format(train_len))
    print ("test_len: {}".format(test_len))
    print ("range(len(x)-train_len-test_len+1): {}".format(range(len(x)-train_len-test_len+1)))
    for i in range(len(x)-train_len-test_len+1):
        in_.append(x[i:(i+train_len), feature_cols].tolist())
        out_.append(x[(i+train_len):(i+train_len+test_len), target_cols].tolist())
        lbl.append(x[i+train_len, label_col])
        #print (">>> i: {}".format(i))
        #print ("  in_: {}".format(in_))
        #print ("  out_: {}".format(out_))
        #print ("  lbl_: {}".format(lbl))
        #print ("\n")

    return np.array(in_), np.array(out_), np.array(lbl)

# linspace(): the function that returns evenly spaced numbers over a specified interval
# the first and second arguments determines an interval. the third argument determines the number of samples.
t = np.linspace(0, 10, 11)

x1 = [i * t for i in range(1, 3)]
x1 = sum(x1)

x2 = [0.5 * i * t for i in range(4, 6)] + np.random.normal(0, 3, 11)
x2 = sum(x2)

x_index = np.array(range(len(t)))

train_ratio = 0.8
train_len = int(train_ratio * t.shape[0])

x1_trend_param_1 = np.polyfit(x_index[:train_len], x1[:train_len], 1)
x1_trend_param_2 = np.polyfit(x_index[:train_len], x1[:train_len], 2)
x1_trend_param_3 = np.polyfit(x_index[:train_len], x1[:train_len], 3)

x1_trend_1 = x_index * x1_trend_param_1[0] + x1_trend_param_1[1]
x1_trend_2 = (x_index**2) * x1_trend_param_2[0] + x_index * x1_trend_param_2[1]
x1_trend_3 = (x_index**3) * x1_trend_param_3[0] + (x_index**2) * x1_trend_param_3[1] + x_index * x1_trend_param_3[2]

x2_trend_param_1 = np.polyfit(x_index[:train_len], x2[:train_len], 1)
x2_trend_param_2 = np.polyfit(x_index[:train_len], x2[:train_len], 2)
x2_trend_param_3 = np.polyfit(x_index[:train_len], x2[:train_len], 3)

x2_trend_1 = x_index * x2_trend_param_1[0] + x2_trend_param_1[1]
x2_trend_2 = (x_index**2) * x2_trend_param_2[0] + x_index * x2_trend_param_2[1]
x2_trend_3 = (x_index**3) * x2_trend_param_3[0] + (x_index**2) * x2_trend_param_3[1] + x_index * x2_trend_param_3[2]

x_lbl = np.column_stack([x1_trend_1, x2_trend_3, x_index, [1]*train_len + [0]*(len(x_index) - train_len)])
print ("x_lbl: {}".format(x_lbl))

x_train_two = x_lbl[x_lbl[:, 3]==1, :2]
x_train_max_0 = x_lbl[x_lbl[:, 3]==1, :2].max(axis=0)
x_train_max_1 = x_lbl[x_lbl[:, 3]==1, :2].max(axis=1)

x_train_max = x_train_max_0
x_train_max = x_train_max.tolist()
x_train_max = x_train_max + [1] * 2

x_normalize = np.divide(x_lbl, x_train_max)
print ("x_normalize: {}".format(x_normalize))

X_in, X_out, lbl = truncate(x_normalize, feature_cols=range(3), target_cols=range(3), label_col=3, train_len=5, test_len=2)

print (">>> X_in")
for i in range(len(X_in)):
    print ("  X_in[{}]: {}".format(i, X_in[i]))
    print ("  X_out[{}]: {}".format(i, X_out[i]))
    print ("  lbl[{}]: {}".format(i, lbl[i]))

print ("\n")
print ("np.where(lbl==1): {}".format(np.where(lbl==1.0)))
print ("np.where(lbl==0): {}".format(np.where(lbl==0.0)))
print ("\n")

X_input_train = X_in[np.where(lbl==1)]
X_output_train = X_out[np.where(lbl==1)]
X_input_test = X_in[np.where(lbl==0)]
X_output_test = X_out[np.where(lbl==0)]
print ("X_input_train: {}".format(X_input_train))
print ("\n")
print ("X_output_train: {}".format(X_output_train))
print ("\n")
print ("X_input_test: {}".format(X_input_test))
print ("\n")
print ("X_output_test: {}".format(X_output_test))
print ("\n")

print ("X_input_train.shape: {}".format(X_input_train.shape))
print ("X_output_train.shape: {}".format(X_output_train.shape))
print ("X_input_test.shape: {}".format(X_input_test.shape))
print ("X_output_test.shape: {}".format(X_output_test.shape))

# Test to understand indexes in the numpy array
print ("X_input_train[:, :, :2]: {}".format(X_input_train[:, :, :2]))
print ("X_input_train[:2, :, :3]: {}".format(X_input_train[:2, :, :3]))
print ("X_input_train[:, :3, :2]: {}".format(X_input_train[:2, :3, :2]))
