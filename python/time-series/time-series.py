import pandas as pd
import numpy as np
import matplotlib.pylab as plt
from datetime import datetime

data = pd.read_csv("AirPassengers.csv")
print (data.head())
print ("  Data Types:")
print (data.dtypes)

con = data["Month"]
data["Month"] = pd.to_datetime(data["Month"])
data.set_index("Month", inplace=True)
print (data.index)

ts = data['#Passengers']
print (ts.head(10))

print ("ts[\'1949-01-01\']")
print (ts['1949-01-01'])

print ("ts[datetime(1949,1,1)]")
print (ts[datetime(1949,1,1)])

print ("ts[\'1949-01-01\':\'1949-05-01\']")
print (ts['1949-01-01':'1949-05-01'])

print ("ts[:\'1949-05-01\']")
print (ts[:'1949-05-01'])

plt.plot(ts)
