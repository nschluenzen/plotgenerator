#include "pyplot.hpp"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <cstring>
pyplot::pyplot(std::vector< std::string > _mRNAs)
{
  mRNAs = _mRNAs;
}

void pyplot::printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument)
{
  printParserErrorDescription(e, file, line, functionArgument, functionArgument);
}

void pyplot::printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument, std::string argument)
{
  std::cerr << "Error in " << file << "::" << line << "(" << functionArgument << "): " << e.what() << " of \"" << argument <<  "\" failed." << std::endl;
}

std::vector<int> pyplot::ParseSelectionString(std::string SelectionString)
{
  std::vector<int> SelectionIndices;
  std::vector<int> EmptyVector;
  std::vector<std::string> SubSelectionStrings;
  std::size_t found = 1;
  int ProjectIndex;
  while (found != std::string::npos)
  {
    found = SelectionString.find(",");
    SubSelectionStrings.push_back(SelectionString.substr(0, found));
    SelectionString = SelectionString.substr(found+1, SelectionString.size()-found-1);
  }
  for (auto SubSelectionString : SubSelectionStrings)
  {
    found = SubSelectionString.find("-");
    if (found == std::string::npos)
    {
      try
      {
	std::stringstream stst(SubSelectionString);
	if ( !(stst >> ProjectIndex) )
	{
	  throw std::invalid_argument("Conversion to int failed.");
	}
      }
      catch(std::exception& e)
      {
	printParserErrorDescription(e, __FILE__, __LINE__, SelectionString, SubSelectionString );
        return EmptyVector;
      }
      std::vector<int>::iterator it = std::find (SelectionIndices.begin(), SelectionIndices.end(), ProjectIndex);
      if (it == SelectionIndices.end())
      {
	SelectionIndices.push_back(ProjectIndex);
      }
    }
    else
    {
      int first;
      int last;
      try
      {
	std::stringstream stst(SubSelectionString.substr(0,found));
	if ( !(stst >> first) )
	{
	  throw std::invalid_argument("Conversion to int failed.");
	}
      }
      catch(std::exception& e)
      {
	printParserErrorDescription(e, __FILE__, __LINE__, SelectionString, SubSelectionString.substr(0, found));
        return EmptyVector;	
      }
      try
      {
	std::stringstream stst(SubSelectionString.substr(found+1,SubSelectionString.size()-found-1));
	if ( !(stst >> last) )
	{
	  throw std::invalid_argument("Conversion to int failed.");
	}
      }
      catch(std::exception& e)
      {
	printParserErrorDescription(e, __FILE__, __LINE__, SelectionString, SubSelectionString.substr(found+1,SubSelectionString.size()-found-1));
	return EmptyVector;
      }
      for (int i=std::min(first,last); i<= std::max(first,last); i++)
      {
	std::vector<int>::iterator it = std::find (SelectionIndices.begin(), SelectionIndices.end(), i);
	if (it == SelectionIndices.end())
	{
	  SelectionIndices.push_back(i);
	}
      }
    }
  }
  return SelectionIndices;
}

std::vector<std::string> pyplot::ChooseIfNeeded(const std::vector<std::string> &PossibleChoices, std::string question, const std::vector<std::string> &Defaults, int RequiredNumberOfArguments)
{
    std::string Choice;
    assert((Defaults.size() == 0) || Defaults.size() >= RequiredNumberOfArguments);
    if (PossibleChoices.size() == 1)
    {
        std::vector<std::string> Choices;
        if (RequiredNumberOfArguments == 1)
        {
            std::cout << question << std::endl;
            std::cout << "Using " << PossibleChoices[0] << "." << std::endl;
            Choices.push_back(PossibleChoices[0]);
        }
        else
        {
            std::cout << "The choice: \"" << question << "\" requires " << RequiredNumberOfArguments << " arguments, but only one given." << std::endl;
        }
        return Choices;
    }
    else
    {
        for (;;)
        {
            std::vector<std::string> Choices;
            bool isCorrect = true;
            std::cout << "Available choices are (\"0\" for exit, 'd' for default): " << std::endl;
            std::vector <int> SelectionInts;
            for (int i = 0; i < PossibleChoices.size(); ++i)
            {
                std::cout << "(" << i+1 << ") ";
                if (std::find(Defaults.begin(),Defaults.end(),PossibleChoices[i]) != Defaults.end()) std::cout << "[";
                std::cout << PossibleChoices[i]; 
                if (std::find(Defaults.begin(),Defaults.end(),PossibleChoices[i]) != Defaults.end()) std::cout << "]";
                std::cout << " " << std::endl;
            }
            std::cout << question << std::endl;
            std::cin >> Choice;
            if (Choice == "d")
            {
                return Defaults;
            }
            else if (Choice == "0")
            {
                return Choices;
            }
            else
            {
                SelectionInts = ParseSelectionString(Choice); // one-based
            }
            if ((SelectionInts.size() > 0) && ((RequiredNumberOfArguments == 0) || (SelectionInts.size() == RequiredNumberOfArguments)))
            {
                for (int i = 0; i < SelectionInts.size(); ++i)
                {
                    if (SelectionInts[i] - 1 < PossibleChoices.size()) // one-based
                    {
                        Choices.push_back(PossibleChoices[SelectionInts[i] - 1]);
                    }
                    else
                    {
                        std::cout << "Value " << SelectionInts[i] << " is out of range (1-" << PossibleChoices.size() << ")" << std::endl;
                        isCorrect = false;
                    }
                }
                if (isCorrect)
                {
                    return Choices;
                }
            }
            else if (SelectionInts.size() == 0)
            {
                // already parser error output
            }
            else // more than required arguments chosen
            {
                std::cout << "Please select " << RequiredNumberOfArguments << " value(s)." << std::endl; 
            }
        }
    }
}

void pyplot::CreatePlotFile()
{
  std::fstream python;
  python.open("plot.py",std::ios::out);
  
  python << "import numpy as np" << std::endl;
  python << "import math" << std::endl;
  python << "from matplotlib import rc, use, get_backend" << std::endl;
  python << "import matplotlib.pyplot as plt" << std::endl;
  python << "from matplotlib.colors import LogNorm" << std::endl;
  python << "import pylab as py" << std::endl;

  python << "def get_xy (name):" << std::endl;
  python << "    data_1 = np.genfromtxt('meta/'+name+'.dat')" << std::endl;
  python << "    mean=0" << std::endl;
  python << "    if (data_1.size != 1):" << std::endl;
  python << "        j=0" << std::endl;
  python << "        for i in range (0,data_1.size):" << std::endl;
  python << "            if (data_1[i] != 0):" << std::endl;
  python << "                j+=1" << std::endl;
  python << "        for i in range (0,data_1.size):" << std::endl;
  python << "            if (data_1[i] != 0):" << std::endl;
  python << "                mean += data_1[i]" << std::endl;
  python << "            else :" << std::endl;
  python << "                data_1[i] = 22.0" << std::endl;
  python << "        mean = mean / j" << std::endl;
  python << "    else:" << std::endl;
  python << "        mean=data_1" << std::endl;
  python << "    x_array = np.arange(0,data_1.size)" << std::endl;
  python << "    y_array = data_1" << std::endl;
  python << "    return (x_array,y_array,mean)" << std::endl;

  python << "def get_xy_cell (name,offset):" << std::endl;
  python << "    data_1 = np.genfromtxt('input/cell_line_'+name+'.dat')" << std::endl;
  python << "    x_array = np.arange(offset,data_1.size+offset)" << std::endl;
  python << "    y_array = data_1" << std::endl;
  python << "    array = []" << std::endl;
  python << "    for i in range (0,data_1.size):" << std::endl;
  python << "        array.append((i+1,x_array[i],y_array[i]))" << std::endl;
  python << "    return (x_array,y_array,array)" << std::endl;

  python << "def plotData (ax,Gene,gtype,abbrev):" << std::endl;
  python << "    ax.set_title(Gene+'\\n'+gtype)" << std::endl;
  python << "    (x_n1,y_n1,m1) = get_xy(Gene+'_'+abbrev+'1')" << std::endl;
  python << "    (x_n2,y_n2,m2) = get_xy(Gene+'_'+abbrev+'2')" << std::endl;
  python << "    (x_n3,y_n3,m3) = get_xy(Gene+'_'+abbrev+'3')" << std::endl;
  python << "    ax.plot(x_n1,y_n1,'ro',label=r'glioblastoma',clip_on=False)" << std::endl;
  python << "    ax.axhline(y=m1,color='r')" << std::endl;
  python << "    ax.plot(x_n2,y_n2,'bo',label=r'recurrence',clip_on=False)" << std::endl;
  python << "    ax.axhline(y=m2,color='b')" << std::endl;
  python << "    ax.plot(x_n3,y_n3,'go',label=r'astrocytoma',clip_on=False)" << std::endl;
  python << "    ax.axhline(y=m3,color='g')" << std::endl;
  python << "    max1 = 0" << std::endl;
  python << "    if (x_n1.size == 1) :" << std::endl;
  python << "        max1 = x_n1" << std::endl;
  python << "    else :" << std::endl;
  python << "        max1 = max(x_n1)" << std::endl;
  python << "    max2 = 0" << std::endl;
  python << "    if (x_n2.size == 1) :" << std::endl;
  python << "        max2 = x_n2" << std::endl;
  python << "    else :" << std::endl;
  python << "        max2 = max(x_n2)" << std::endl;
  python << "    max3 = 0" << std::endl;
  python << "    if (x_n3.size == 1) :" << std::endl;
  python << "        max3 = x_n3" << std::endl;
  python << "    else :" << std::endl;
  python << "        max3 = max(x_n3)" << std::endl;
  python << "    (x_cell,y_cell,array) = get_xy_cell(Gene,max(max(max1,max2),max3)+2)" << std::endl;
  python << "    ax.plot(x_cell,y_cell,'dk',label=r'cell lines',clip_on=False,markersize=7)" << std::endl;
  python << "    for item in array:" << std::endl;
  python << "        ax.scatter(item[1], item[2], marker=r\"$ {} $\".format(item[0]),c='w',edgecolors='none',zorder=10)" << std::endl;
  python << "    ax.axhline(y=21.0,color='k',label='below detection\\nlimit')" << std::endl;
  python << "    ax.tick_params(" << std::endl;
  python << "        axis='x',          # changes apply to the x-axis" << std::endl;
  python << "        which='both',      # both major and minor ticks are affected" << std::endl;
  python << "        bottom='off',      # ticks along the bottom edge are off" << std::endl;
  python << "        top='off',         # ticks along the top edge are off" << std::endl;
  python << "        labelbottom='off') # labels along the bottom edge are off" << std::endl;
  python << "    ax.set_xlim(-1,max(x_cell)+1)" << std::endl;
  python << "    ax.set_ylim(-5,23)" << std::endl;
  python << "    min1 = 0" << std::endl;
  python << "    if (y_n1.size == 1) :" << std::endl;
  python << "        min1 = y_n1" << std::endl;
  python << "    else :" << std::endl;
  python << "        min1 = min(y_n1)" << std::endl;
  python << "    min2 = 0" << std::endl;
  python << "    if (y_n2.size == 1) :" << std::endl;
  python << "        min2 = y_n2" << std::endl;
  python << "    else :" << std::endl;
  python << "        min2 = min(y_n2)" << std::endl;
  python << "    min3 = 0" << std::endl;
  python << "    if (y_n3.size == 1) :" << std::endl;
  python << "        min3 = y_n3" << std::endl;
  python << "    else :" << std::endl;
  python << "        min3 = min(y_n3)" << std::endl;
  python << "    return min(min(min1,min2),min3)" << std::endl;
  
  python << "def CreateMultiPlot(Gene):" << std::endl;
  python << "    rc('font', family='serif')" << std::endl;
  python << "    rc('font', serif = 'cmr10')" << std::endl;
  python << "    rc('font', size=12)" << std::endl;
  python << "    rc('text', usetex=True)" << std::endl;
  python << "    fig, ((ax1, ax2, ax3, ax4)) = plt.subplots(1,4, sharey=True)" << std::endl;
  python << "    size = fig.get_size_inches()" << std::endl;
  python << "    fig.set_size_inches((0.9*size[0],0.7*size[1]))" << std::endl;
  python << "    plt.subplots_adjust(left=0.08,right=0.97,top=0.88,bottom=0.04)" << std::endl;
  python << "    ax1.set_ylabel(r'$\\Delta$CT value')" << std::endl;
  python << "    data_min1 = plotData(ax1,Gene,'native','n')" << std::endl;
  python << "    data_min2 = plotData(ax2,Gene,'primary culture','p')" << std::endl;
  python << "    data_min3 = plotData(ax3,Gene,'upper fraction','o')" << std::endl;
  python << "    data_min4 = plotData(ax4,Gene,'lower fraction','u')" << std::endl;
  python << "    if (data_min1 > 4):" << std::endl;
  python << "        ax1.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << std::endl;
  python << "    else:" << std::endl;
  python << "        if (data_min4 > 4):" << std::endl;
  python << "            ax4.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << std::endl;
  python << "        else:" << std::endl;
  python << "            if (data_min2 > 4):" << std::endl;
  python << "                ax2.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << std::endl;
  python << "            else:" << std::endl;
  python << "                if (data_min3 > 4):" << std::endl;
  python << "                    ax3.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << std::endl;
  python << "                else:" << std::endl;
  python << "                    ax1.legend(fancybox=True,loc='best',ncol=1,fontsize=8,numpoints=1)" << std::endl;
  python << "    ax1.set_ylim(ax1.get_ylim()[::-1])" << std::endl;
  python << "    plt.savefig('output/'+Gene+'.pdf')" << std::endl;
    
  auto chosenmRNAs = ChooseIfNeeded(mRNAs,"which plots should be created?",mRNAs,0);
  
  std::cout << "Create plots for:";
  for (auto chosenmRNA : chosenmRNAs)
  {
    std::cout << " " << chosenmRNA;
  }
  std::cout << std::endl;
  for (auto mRNA : chosenmRNAs)
  {
    python << "CreateMultiPlot('" << mRNA << "')" << std::endl;
  }
  
  python.close();
}

void pyplot::plot()
{
  CreatePlotFile();
  std::string remove = "del";
  system("python plot.py");
  system((remove+" plot.py").c_str());
}
