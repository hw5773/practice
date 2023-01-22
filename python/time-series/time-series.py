import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from tqdm import trange
import random

data = pd.read_csv("data.csv", encoding="CP949")
plt.figure(figsize=(20, 5))
plt.plot(range(len(data)), data["평균속도"])
print (data.head())
