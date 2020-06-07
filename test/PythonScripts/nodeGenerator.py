# Node generating script
num_nodes = 100
print("*Node speed test")
print("V1 1 0 SINE(0 100 1)")
for i in range(1,num_nodes):
    resistorSmall = "R" + str(i) + " " + str(i) + " " + str(i + 1) + " 1"
    resistorLarge = "R" + str(i+100) + " " + str(i) + " " + str(0) + " 1000"
    print(resistorSmall)
    print(resistorLarge)
print(".tran 0 3 0 0.001")
print(".end")
