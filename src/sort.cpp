#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex>
#include <vector>
#include <utility>
#include <libssh/libssh.h>
#include <algorithm>

std::vector<std::pair<std::string,std::vector<double>>> getRNA_Data(std::string filename)
{
  std::vector<std::string> mRNAs;
  std::vector<std::pair<std::string,std::vector<double>>> mRNAData;
  mRNAs.push_back("ITGA5");
  mRNAs.push_back("ITGB4");
  mRNAs.push_back("CXCR4");
  mRNAs.push_back("CXCR7");
  mRNAs.push_back("FN1");
  mRNAs.push_back("GFAP");
  mRNAs.push_back("Vimentin");
  mRNAs.push_back("Snail1");
  mRNAs.push_back("Snail2");
  mRNAs.push_back("Twist1");
//   mRNAs.push_back("Desmoplakin");
  
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
      Data.push_back(std::stod(value));
      line = line.substr(pos+1);
//       pos = line.find("\t");
//       if (pos == std::string::npos)
//       {
// 	Data.push_back(std::stod(line));
//       }
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

void WriteRNA_Data(std::vector<std::pair<std::string,std::vector<double>>> mRNAData, std::string name)
{
  for (auto Data : mRNAData)
  {
    std::fstream datastream;
    datastream.open(std::get<0>(Data)+"_"+name+".dat",std::ios::out);
    for (auto n1DData : std::get<1>(Data))
    {
      datastream << n1DData << std::endl;
    }
    datastream.close();
  }
}

main()
{
//   std::cout << "nativ" << std::endl;
  auto mRNAData_nativ = getRNA_Data("nativ");
//   std::cout << "nativ2" << std::endl;
  auto mRNAData_nativ2 = getRNA_Data("nativ2");
  auto mRNAData_nativ3 = getRNA_Data("nativ3");
//   std::cout << "primär" << std::endl;
  auto mRNAData_primaer = getRNA_Data("primär");
//   std::cout << "primär2" << std::endl;
  auto mRNAData_primaer2 = getRNA_Data("primär2");
  auto mRNAData_primaer3 = getRNA_Data("primär3");
//   std::cout << "oben" << std::endl;
  auto mRNAData_oben = getRNA_Data("oben");
//   std::cout << "oben2" << std::endl;
  auto mRNAData_oben2 = getRNA_Data("oben2");
  auto mRNAData_oben3 = getRNA_Data("oben3");
//   std::cout << "unten" << std::endl;
  auto mRNAData_unten = getRNA_Data("unten");
//   std::cout << "unten2" << std::endl;
  auto mRNAData_unten2 = getRNA_Data("unten2"); 
  auto mRNAData_unten3 = getRNA_Data("unten3"); 
  
  WriteRNA_Data(mRNAData_nativ,"n1");
  WriteRNA_Data(mRNAData_nativ2,"n2");
  WriteRNA_Data(mRNAData_nativ3,"n3");
  WriteRNA_Data(mRNAData_primaer,"p1");
  WriteRNA_Data(mRNAData_primaer2,"p2");
  WriteRNA_Data(mRNAData_primaer3,"p3");
  WriteRNA_Data(mRNAData_oben,"o1");
  WriteRNA_Data(mRNAData_oben2,"o2");
  WriteRNA_Data(mRNAData_oben3,"o3");
  WriteRNA_Data(mRNAData_unten,"u1");
  WriteRNA_Data(mRNAData_unten2,"u2");
  WriteRNA_Data(mRNAData_unten3,"u3");
  
}