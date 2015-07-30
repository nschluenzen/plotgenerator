import numpy as np
import math
from matplotlib import rc, use, get_backend
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import pylab as py

#ax.set_xlabel(r'Data points')

def get_xy (name):
    data_1 = np.genfromtxt('./'+name+'.dat')
    mean=0
    if (data_1.size != 1):
        j=0
        for i in range (0,data_1.size):
            if (data_1[i] != 0):
                j+=1
        for i in range (0,data_1.size):
            if (data_1[i] != 0):
                mean += data_1[i]
            else :
                data_1[i] = 22.0
        mean = mean / j
    else:
        mean=data_1
    x_array = np.arange(0,data_1.size)
    y_array = data_1
    return (x_array,y_array,mean)

def get_xy_cell (name,offset):
    data_1 = np.genfromtxt('./cell_line_'+name+'.dat')
    x_array = np.arange(offset,data_1.size+offset)
    y_array = data_1
    array = []
    for i in range (0,data_1.size):
        array.append((i+1,x_array[i],y_array[i]))
    return (x_array,y_array,array)

#def get_xy_cell (name,offset):
    #data_1 = np.genfromtxt('./cell_line_'+name+'.dat')
    #x_array = np.arange(offset,data_1[1,:].size+offset)
    #y_array = data_1[1,:]
    #labels = data_1[0,:]
    #array = []
    #for i in range (0,data_1[1,:].size):
        #array.append((offset+i,data_1[0,i],data_1[1,i]))
    #return (array)

def plotData (ax,Gene,gtype,abbrev):
    ax.set_title(Gene+'\n'+gtype)

    (x_n1,y_n1,m1) = get_xy(Gene+'_'+abbrev+'1')
    (x_n2,y_n2,m2) = get_xy(Gene+'_'+abbrev+'2')
    (x_n3,y_n3,m3) = get_xy(Gene+'_'+abbrev+'3')
    ax.plot(x_n1,y_n1,'ro',label=r'glioblastoma',clip_on=False)
    ax.axhline(y=m1,color='r')
    ax.plot(x_n2,y_n2,'bo',label=r'recurrence',clip_on=False)
    ax.axhline(y=m2,color='b')
    ax.plot(x_n3,y_n3,'go',label=r'astrocytoma',clip_on=False)
    ax.axhline(y=m3,color='g')
    
    max1 = 0
    if (x_n1.size == 1) :
        max1 = x_n1
    else :
        max1 = max(x_n1)
    max2 = 0
    if (x_n2.size == 1) :
        max2 = x_n2
    else :
        max2 = max(x_n2)
    max3 = 0
    if (x_n3.size == 1) :
        max3 = x_n3
    else :
        max3 = max(x_n3)
    (x_cell,y_cell,array) = get_xy_cell(Gene,max(max(max1,max2),max3)+2)
    #(array) = get_xy_cell('ITGA5',max(max(x_n1),max(x_n2))+2)
    #print(array)
    ax.plot(x_cell,y_cell,'dk',label=r'cell lines',clip_on=False,markersize=7)
    for item in array:
        ax.scatter(item[1], item[2], marker=r"$ {} $".format(item[0]),c='w',edgecolors='none',zorder=10)

    ax.axhline(y=21.0,color='k',label='below detection\nlimit')

    ax.tick_params(
        axis='x',          # changes apply to the x-axis
        which='both',      # both major and minor ticks are affected
        bottom='off',      # ticks along the bottom edge are off
        top='off',         # ticks along the top edge are off
        labelbottom='off') # labels along the bottom edge are off

    ax.set_xlim(-1,max(x_cell)+1)
    ax.set_ylim(-5,23)
    #ax.set_ylim(ax.get_ylim()[::-1])
    
    min1 = 0
    if (y_n1.size == 1) :
        min1 = y_n1
    else :
        min1 = min(y_n1)
    min2 = 0
    if (y_n2.size == 1) :
        min2 = y_n2
    else :
        min2 = min(y_n2)
    min3 = 0
    if (y_n3.size == 1) :
        min3 = y_n3
    else :
        min3 = min(y_n3)
    
    return min(min(min1,min2),min3)

def CreateMultiPlot(Gene):
    rc('font', family='serif')
    rc('font', serif = 'cmr10')
    rc('font', size=12)
    rc('text', usetex=True)
    fig, ((ax1, ax2, ax3, ax4)) = plt.subplots(1,4, sharey=True)
    size = fig.get_size_inches()
    fig.set_size_inches((0.9*size[0],0.7*size[1]))
    plt.subplots_adjust(left=0.08,right=0.97,top=0.88,bottom=0.04)

    #ax2.set_ylim(ax2.get_ylim()[::-1])
    #ax3.set_ylim(ax3.get_ylim()[::-1])
    #ax4.set_ylim(ax4.get_ylim()[::-1])
    
    ax1.set_ylabel(r'$\Delta$CT value')

    data_min1 = plotData(ax1,Gene,'native','n')
    data_min2 = plotData(ax2,Gene,'primary culture','p')
    data_min3 = plotData(ax3,Gene,'upper fraction','o')
    data_min4 = plotData(ax4,Gene,'lower fraction','u')
    if (data_min1 > 4):
        ax1.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)
    else:
        if (data_min4 > 4):
            ax4.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)
        else:
            if (data_min2 > 4):
                ax2.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)
            else:
                if (data_min3 > 4):
                    ax3.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)
                else:
                    ax1.legend(fancybox=True,loc='best',ncol=1,fontsize=8,numpoints=1)
    ax1.set_ylim(ax1.get_ylim()[::-1])
    plt.savefig(Gene+'.pdf')
    
CreateMultiPlot('ITGA5')
CreateMultiPlot('ITGB4')
CreateMultiPlot('CXCR4')
CreateMultiPlot('CXCR7')
CreateMultiPlot('FN1')
CreateMultiPlot('GFAP')
CreateMultiPlot('Vimentin')
CreateMultiPlot('Snail1')
CreateMultiPlot('Snail2')
CreateMultiPlot('Twist1')
#CreateMultiPlot('Desmoplakin')

#handles, labels = ax.get_legend_handles_labels()
#handles2=handles
#labels2 = labels
#handle0 = handles[0]
#label0 = labels[0]
#handle1 = handles[1]
#label1 = labels[1]
#handle2 = handles[2]
#label2 = labels[2]

#handles2[0] = handle0
#labels2[0] = label0
#handles2[1] = handle2
#labels2[1] = label2
#handles2[2] = handle1
#labels2[2] = label1

#ax.grid(color='grey')
