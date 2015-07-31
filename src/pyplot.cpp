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
  std::cerr << "Error in " << file << "::" << line << "(" << functionArgument << "): " << e.what() << " of \"" << argument <<  "\" failed." << "\n";
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
            std::cout << question << "\n";
            std::cout << "Using " << PossibleChoices[0] << "." << "\n";
            Choices.push_back(PossibleChoices[0]);
        }
        else
        {
            std::cout << "The choice: \"" << question << "\" requires " << RequiredNumberOfArguments << " arguments, but only one given." << "\n";
        }
        return Choices;
    }
    else
    {
        for (;;)
        {
            std::vector<std::string> Choices;
            bool isCorrect = true;
            std::cout << "Available choices are (\"0\" for exit, 'd' for default): " << "\n";
            std::vector <int> SelectionInts;
            for (int i = 0; i < PossibleChoices.size(); ++i)
            {
                std::cout << "(" << i+1 << ") ";
                if (std::find(Defaults.begin(),Defaults.end(),PossibleChoices[i]) != Defaults.end()) std::cout << "[";
                std::cout << PossibleChoices[i]; 
                if (std::find(Defaults.begin(),Defaults.end(),PossibleChoices[i]) != Defaults.end()) std::cout << "]";
                std::cout << " " << "\n";
            }
            std::cout << question << "\n";
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
                        std::cout << "Value " << SelectionInts[i] << " is out of range (1-" << PossibleChoices.size() << ")" << "\n";
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
                std::cout << "Please select " << RequiredNumberOfArguments << " value(s)." << "\n"; 
            }
        }
    }
}

void pyplot::CreatePlotFile()
{
  std::fstream python;
  python.open("plot.py",std::ios::out);
  
  python << "import numpy as np" << "\n";
  python << "import math" << "\n";
  python << "from matplotlib import rc, use, get_backend" << "\n";
  python << "import matplotlib.pyplot as plt" << "\n";
  python << "from matplotlib.colors import LogNorm" << "\n";
  python << "import pylab as py" << "\n";

  python << "def get_xy (name):" << "\n";
  python << "    data_1 = np.genfromtxt('meta/'+name+'.dat')" << "\n";
  python << "    mean=0" << "\n";
  python << "    if (data_1.size != 1):" << "\n";
  python << "        j=0" << "\n";
  python << "        for i in range (0,data_1.size):" << "\n";
  python << "            if (data_1[i] != 0):" << "\n";
  python << "                j+=1" << "\n";
  python << "        for i in range (0,data_1.size):" << "\n";
  python << "            if (data_1[i] != 0):" << "\n";
  python << "                mean += data_1[i]" << "\n";
  python << "            else :" << "\n";
  python << "                data_1[i] = 22.0" << "\n";
  python << "        mean = mean / j" << "\n";
  python << "    else:" << "\n";
  python << "        mean=data_1" << "\n";
  python << "    x_array = np.arange(0,data_1.size)" << "\n";
  python << "    y_array = data_1" << "\n";
  python << "    return (x_array,y_array,mean)" << "\n";

  python << "def get_xy_cell (name,offset):" << "\n";
  python << "    data_1 = np.genfromtxt('input/cell_line_'+name+'.dat')" << "\n";
  python << "    x_array = np.arange(offset,data_1.size+offset)" << "\n";
  python << "    y_array = data_1" << "\n";
  python << "    array = []" << "\n";
  python << "    for i in range (0,data_1.size):" << "\n";
  python << "        array.append((i+1,x_array[i],y_array[i]))" << "\n";
  python << "    return (x_array,y_array,array)" << "\n";

  python << "def plotData (ax,Gene,gtype,abbrev):" << "\n";
  python << "    ax.set_title(Gene+'\\n'+gtype)" << "\n";
  python << "    (x_n1,y_n1,m1) = get_xy(Gene+'_'+abbrev+'1')" << "\n";
  python << "    (x_n2,y_n2,m2) = get_xy(Gene+'_'+abbrev+'2')" << "\n";
  python << "    (x_n3,y_n3,m3) = get_xy(Gene+'_'+abbrev+'3')" << "\n";
  python << "    ax.plot(x_n1,y_n1,'ro',label=r'glioblastoma',clip_on=False)" << "\n";
  python << "    ax.axhline(y=m1,color='r')" << "\n";
  python << "    ax.plot(x_n2,y_n2,'bo',label=r'recurrence',clip_on=False)" << "\n";
  python << "    ax.axhline(y=m2,color='b')" << "\n";
  python << "    ax.plot(x_n3,y_n3,'go',label=r'astrocytoma',clip_on=False)" << "\n";
  python << "    ax.axhline(y=m3,color='g')" << "\n";
  python << "    max1 = 0" << "\n";
  python << "    if (x_n1.size == 1) :" << "\n";
  python << "        max1 = x_n1" << "\n";
  python << "    else :" << "\n";
  python << "        max1 = max(x_n1)" << "\n";
  python << "    max2 = 0" << "\n";
  python << "    if (x_n2.size == 1) :" << "\n";
  python << "        max2 = x_n2" << "\n";
  python << "    else :" << "\n";
  python << "        max2 = max(x_n2)" << "\n";
  python << "    max3 = 0" << "\n";
  python << "    if (x_n3.size == 1) :" << "\n";
  python << "        max3 = x_n3" << "\n";
  python << "    else :" << "\n";
  python << "        max3 = max(x_n3)" << "\n";
  python << "    (x_cell,y_cell,array) = get_xy_cell(Gene,max(max(max1,max2),max3)+2)" << "\n";
  python << "    ax.plot(x_cell,y_cell,'dk',label=r'cell lines',clip_on=False,markersize=7)" << "\n";
  python << "    for item in array:" << "\n";
  python << "        ax.scatter(item[1], item[2], marker=r\"$ {} $\".format(item[0]),c='w',edgecolors='none',zorder=10)" << "\n";
  python << "    ax.axhline(y=21.0,color='k',label='below detection\\nlimit')" << "\n";
  python << "    ax.tick_params(" << "\n";
  python << "        axis='x',          # changes apply to the x-axis" << "\n";
  python << "        which='both',      # both major and minor ticks are affected" << "\n";
  python << "        bottom='off',      # ticks along the bottom edge are off" << "\n";
  python << "        top='off',         # ticks along the top edge are off" << "\n";
  python << "        labelbottom='off') # labels along the bottom edge are off" << "\n";
  python << "    ax.set_xlim(-1,max(x_cell)+1)" << "\n";
  python << "    ax.set_ylim(-5,23)" << "\n";
  python << "    min1 = 0" << "\n";
  python << "    if (y_n1.size == 1) :" << "\n";
  python << "        min1 = y_n1" << "\n";
  python << "    else :" << "\n";
  python << "        min1 = min(y_n1)" << "\n";
  python << "    min2 = 0" << "\n";
  python << "    if (y_n2.size == 1) :" << "\n";
  python << "        min2 = y_n2" << "\n";
  python << "    else :" << "\n";
  python << "        min2 = min(y_n2)" << "\n";
  python << "    min3 = 0" << "\n";
  python << "    if (y_n3.size == 1) :" << "\n";
  python << "        min3 = y_n3" << "\n";
  python << "    else :" << "\n";
  python << "        min3 = min(y_n3)" << "\n";
  python << "    return min(min(min1,min2),min3)" << "\n";
  
  python << "def CreateMultiPlot(Gene):" << "\n";
  python << "    rc('font', family='serif')" << "\n";
  python << "    rc('font', serif = 'cmr10')" << "\n";
  python << "    rc('font', size=12)" << "\n";
  python << "    rc('text', usetex=True)" << "\n";
  python << "    fig, ((ax1, ax2, ax3, ax4)) = plt.subplots(1,4, sharey=True)" << "\n";
  python << "    size = fig.get_size_inches()" << "\n";
  python << "    fig.set_size_inches((0.9*size[0],0.7*size[1]))" << "\n";
  python << "    plt.subplots_adjust(left=0.08,right=0.97,top=0.88,bottom=0.04)" << "\n";
  python << "    ax1.set_ylabel(r'$\\Delta$CT value')" << "\n";
  python << "    data_min1 = plotData(ax1,Gene,'native','n')" << "\n";
  python << "    data_min2 = plotData(ax2,Gene,'primary culture','p')" << "\n";
  python << "    data_min3 = plotData(ax3,Gene,'upper fraction','o')" << "\n";
  python << "    data_min4 = plotData(ax4,Gene,'lower fraction','u')" << "\n";
  python << "    if (data_min1 > 4):" << "\n";
  python << "        ax1.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << "\n";
  python << "    else:" << "\n";
  python << "        if (data_min4 > 4):" << "\n";
  python << "            ax4.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << "\n";
  python << "        else:" << "\n";
  python << "            if (data_min2 > 4):" << "\n";
  python << "                ax2.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << "\n";
  python << "            else:" << "\n";
  python << "                if (data_min3 > 4):" << "\n";
  python << "                    ax3.legend(fancybox=True,loc='upper center',ncol=1,fontsize=8,numpoints=1)" << "\n";
  python << "                else:" << "\n";
  python << "                    ax1.legend(fancybox=True,loc='best',ncol=1,fontsize=8,numpoints=1)" << "\n";
  python << "    ax1.set_ylim(ax1.get_ylim()[::-1])" << "\n";
  python << "    plt.savefig('output/'+Gene+'.pdf')" << "\n";
    
  auto chosenmRNAs = ChooseIfNeeded(mRNAs,"which plots should be created?",mRNAs,0);
  
  std::cout << "Create plots for:";
  for (auto chosenmRNA : chosenmRNAs)
  {
    std::cout << " " << chosenmRNA;
  }
  std::cout << "\n";
  for (auto mRNA : chosenmRNAs)
  {
    python << "CreateMultiPlot('" << mRNA << "')" << "\n";
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
