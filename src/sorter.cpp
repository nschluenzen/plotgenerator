#include "sorter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

sorter::sorter(std::vector< std::string > _mRNAs)
{
  mRNAs = _mRNAs;
}

std::vector<std::pair<std::string,std::vector<double>>> sorter::getRNA_Data(std::string filename)
{
  std::vector<std::pair<std::string,std::vector<double>>> mRNAData;
  
  for (int i = 0; i < mRNAs.size(); i++)
  {
    std::fstream nativ;
    std::string line;
    std::vector<double> Data;
    nativ.open(filename+".dat",std::ios::in);
    for (int j = 0; j <= i; j++)
    {
      std::getline(nativ,line);
    }
    nativ.close();
    std::size_t pos = 1;
    while (pos != std::string::npos)
    { 
      pos = line.find("\t");
      std::string value = line.substr(0,pos);
      std::stringstream stst;
      stst << value;
      Data.push_back(stst.str());
      line = line.substr(pos+1);
    }
    mRNAData.push_back(std::pair<std::string,std::vector<double>>(mRNAs[i],Data));
  }
  
  for (auto mRNA : mRNAData)
  {
    std::cout << std::get<0>(mRNA) << ": ";
    for (auto DataPoint : std::get<1>(mRNA))
    {
      std::cout << DataPoint << " ";
    }
    std::cout << std::endl;
  }   
  
  return mRNAData;
}

void sorter::WriteRNA_Data(std::vector<std::pair<std::string,std::vector<double>>> mRNAData, std::string name)
{
  for (auto Data : mRNAData)
  {
    std::fstream datastream;
    datastream.open("meta/"+std::get<0>(Data)+"_"+name+".dat",std::ios::out);
    for (auto n1DData : std::get<1>(Data))
    {
      datastream << n1DData << std::endl;
    }
    datastream.close();
  }
}