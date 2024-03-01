import matplotlib.pyplot as plt

# Sample data for lines
x = [1, 2, 3, 4, 5]
y=[]

# Open a file for reading
with open('result', 'r') as file:
    # Iterate over each line in the file
    cnt=-1
    for line in file:
        cnt+=1
        if cnt==0:
            lst=[int(x) for x in line.split()]
            x=[x for x in range(1,lst[1]+1)]
            continue
        y.append([float(x) for x in line.split()])
        
colour=['blue','green','red','orange','purple','cyan','magenta']
labels=['TASLock', "TTASLock", "BackoffLock", "ALock", "CLHLock", "MCSLock", "TOLock"]
for i in range(7):
    plt.plot(x, y[i], color=colour[i])
    plt.xlabel('Number of threads')
    plt.ylabel('Perfomance 1/time')
    plt.title(labels[i]+' perfomance')
    plt.text(max(x), max(y[0]), '', horizontalalignment='right', verticalalignment='top')
    plt.tick_params(axis='y', left=False)  
    plt.gca().set_yticklabels([''] * len(y))
    plt.savefig("graphs/"+labels[i]+".png")
    plt.clf()

# Plot the data for each line
plt.plot(x, y[0], color='blue', label='TASLock')
plt.plot(x, y[1], color='green', label='TTASLock')
plt.plot(x, y[2], color='red', label='BackoffLock')
plt.plot(x, y[3], color='orange', label='Alock')
plt.plot(x, y[4], color='purple', label='CLHLock')
plt.plot(x, y[5], color='cyan', label='MCSLock')
plt.plot(x, y[6], color='magenta', label='TOLock')

# Add labels and title
plt.xlabel('Number of threads')
plt.ylabel('Perfomance 1/time')
plt.title('Lock perfomance')
plt.text(max(x), max(y[0]), 'Top Right', horizontalalignment='right', verticalalignment='top')
plt.tick_params(axis='y', left=False)  
plt.gca().set_yticklabels([''] * len(y))  
# plt.savefig('graphs/allLocks.png')
# Add a legend
plt.legend()
# Show the plot
plt.show()