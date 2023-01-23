import numpy as np
import matplotlib.pyplot as plt

print ("50*np.pi: {}".format(50*np.pi))
print ("50*np.pi/9: {}".format(50*np.pi/9))
# linspace(): the function that returns evenly spaced numbers over a specified interval
# the first and second arguments determines an interval. the third argument determines the number of samples.
t = np.linspace(0, 50*np.pi, 10)
print ("t1: {}".format(t))
print ("t1.shape: {}".format(t.shape))
print ("\n")

t = np.linspace(0, 10, 11)
print ("t2: {}".format(t))
print ("t2.shape: {}".format(t.shape))
print ("\n")

x1 = [i * t for i in range(1, 3)]
print ("x1: {}".format(x1))
x1 = sum(x1)
print ("x1(sum): {}".format(x1))

x2 = [0.5 * i * t for i in range(4, 6)] + np.random.normal(0, 3, 11)
print ("x2: {}".format(x2))
x2 = sum(x2)
print ("x2(sum): {}".format(x2))

x_index = np.array(range(len(t)))
print ("x_index: {}".format(x_index))

train_ratio = 0.8
train_len = int(train_ratio * t.shape[0])
print ("train_len: {}".format(train_len))
print ("x_index[:train_len]: {}".format(x_index[:train_len]))
print ("x1[:train_len]: {}".format(x1[:train_len]))

x1_trend_param_1 = np.polyfit(x_index[:train_len], x1[:train_len], 1)
print ("x1_trend_param_1: {}".format(x1_trend_param_1))

x1_trend_param_2 = np.polyfit(x_index[:train_len], x1[:train_len], 2)
print ("x1_trend_param_2: {}".format(x1_trend_param_2))

x1_trend_param_3 = np.polyfit(x_index[:train_len], x1[:train_len], 3)
print ("x1_trend_param_3: {}".format(x1_trend_param_3))

x1_trend_1 = x_index * x1_trend_param_1[0] + x1_trend_param_1[1]
print ("x1_trend_1: {}".format(x1_trend_1))
x1_trend_2 = (x_index**2) * x1_trend_param_2[0] + x_index * x1_trend_param_2[1]
print ("x1_trend_2: {}".format(x1_trend_2))
x1_trend_3 = (x_index**3) * x1_trend_param_3[0] + (x_index**2) * x1_trend_param_3[1] + x_index * x1_trend_param_3[2]
print ("x1_trend_3: {}".format(x1_trend_3))

x2_trend_param_1 = np.polyfit(x_index[:train_len], x2[:train_len], 1)
print ("x2_trend_param_1: {}".format(x2_trend_param_1))

x2_trend_param_2 = np.polyfit(x_index[:train_len], x2[:train_len], 2)
print ("x2_trend_param_2: {}".format(x2_trend_param_2))

x2_trend_param_3 = np.polyfit(x_index[:train_len], x2[:train_len], 3)
print ("x2_trend_param_3: {}".format(x2_trend_param_3))

x2_trend_1 = x_index * x2_trend_param_1[0] + x2_trend_param_1[1]
print ("x2_trend_1: {}".format(x2_trend_1))
x2_trend_2 = (x_index**2) * x2_trend_param_2[0] + x_index * x2_trend_param_2[1]
print ("x2_trend_2: {}".format(x2_trend_2))
x2_trend_3 = (x_index**3) * x2_trend_param_3[0] + (x_index**2) * x2_trend_param_3[1] + x_index * x2_trend_param_3[2]
print ("x2_trend_3: {}".format(x2_trend_3))

plt.figure(figsize=(15, 5))
plt.plot(range(len(x2)), x2, label="x2")
plt.plot(range(len(x2_trend_1)), x2_trend_1, linestyle="--", color="red", label="x2_trend_1")
plt.plot(range(len(x2_trend_2)), x2_trend_2, linestyle=":", color="orange", label="x2_trend_2")
plt.plot(range(len(x2_trend_3)), x2_trend_3, linestyle="dotted", color="green", label="x2_trend_3")
plt.legend(loc="upper center", bbox_to_anchor=(0.5, -0.15), fancybox=True, shadow=False, ncol=2)
plt.show()
