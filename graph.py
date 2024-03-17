import matplotlib.pyplot as plt

# Sample data for lines
x = [1, 2, 3, 4, 5]
y=[]
k=20

# Open a file for reading
with open('result', 'r') as file:
    # Iterate over each line in the file
    cnt=-1
    for line in file:
        cnt+=1
        if cnt==0:
            lst=[int(x) for x in line.split()]
            x=[x for x in range(k,lst[1]+1,k)]
            continue
        y.append([float(x) for x in line.split()])
        
colour=['blue','green','red','purple','cyan',]
labels=['TASLock', "TTASLock", "ALock", "CLHLock", "MCSLock",]
for i in range(5):
    plt.plot(x, y[i], color=colour[i],linewidth=3)
    plt.xlabel('Number of threads')
    plt.ylabel('time(in ms)')
    plt.title(labels[i]+' perfomance')
    # plt.text(max(x), max(y[0]), '', horizontalalignment='right', verticalalignment='top')
    plt.savefig("graphs/"+labels[i]+".pdf",format='pdf')
    plt.clf()

# Plot the data for each line
plt.plot(x, y[0], color='blue', label='TASLock',linewidth=3)
plt.plot(x, y[1], color='green', label='TTASLock',linewidth=3)
plt.plot(x, y[2], color='orange', label='Alock',linewidth=3)
plt.plot(x, y[3], color='purple', label='CLHLock',linewidth=3)
plt.plot(x, y[4], color='cyan', label='MCSLock',linewidth=3)

# Add labels and title
plt.xlabel('Number of threads')
plt.ylabel('time(in ms)')
plt.title('Lock perfomance')
# plt.text(max(x), max(y[0]), 'Top Right', horizontalalignment='right', verticalalignment='top') 
plt.savefig("graphs/allLocks.pdf",format='pdf')
# Add a legend
plt.legend()
# Show the plot
plt.show()