import pandas as pd
import matplotlib.pyplot as plot

def compare(lt,out):
    a = pd.read_csv(lt)
    b = pd.read_csv(out)
    a.set_index('time')
    b.set_index('Time')
    c = a.copy()
    for i in a.columns:
        if i == "time" or i=="Time":
            continue
        else:
            c[i] = abs(a[i]-b[i])
    return c


a = compare("../RC_test/LT.csvs/newR=200,.csv","../RC_test/out.csvs/R=200,.csv")
b = a.plot(kind='scatter',marker='x',x='time',y='V(N001)',color='red')
b.grid(color='black', linestyle='-', linewidth=0.4)
b.set(xlabel="Time /s", ylabel="Error Voltage /V",title="Error Voltage /V Across Resistor vs Time /s\n for a resistor value of 200Ω") 
