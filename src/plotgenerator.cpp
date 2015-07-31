#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include "sorter.hpp"
#include "pyplot.hpp"

std::vector<std::string> LoadmRNAs()
{
  std::vector<std::string> mRNAs;
  std::fstream infile;
  infile.open("mRNAs.in",std::ios::in);
  std::string current_mRNA;
  while(std::getline(infile,current_mRNA))
  {
    mRNAs.push_back(current_mRNA);
  }
  infile.close();
  return mRNAs;
}

void CreateDataSet(std::vector<std::string> mRNAs)
{
  sorter sort(mRNAs);
  auto mRNAData_nativ = sort.getRNA_Data("input/nativ");
  auto mRNAData_nativ2 = sort.getRNA_Data("input/nativ2");
  auto mRNAData_nativ3 = sort.getRNA_Data("input/nativ3");
  auto mRNAData_primaer = sort.getRNA_Data("input/primär");
  auto mRNAData_primaer2 = sort.getRNA_Data("input/primär2");
  auto mRNAData_primaer3 = sort.getRNA_Data("input/primär3");
  auto mRNAData_oben = sort.getRNA_Data("input/oben");
  auto mRNAData_oben2 = sort.getRNA_Data("input/oben2");
  auto mRNAData_oben3 = sort.getRNA_Data("input/oben3");
  auto mRNAData_unten = sort.getRNA_Data("input/unten");
  auto mRNAData_unten2 = sort.getRNA_Data("input/unten2"); 
  auto mRNAData_unten3 = sort.getRNA_Data("input/unten3"); 
  
  sort.WriteRNA_Data(mRNAData_nativ,"n1");
  sort.WriteRNA_Data(mRNAData_nativ2,"n2");
  sort.WriteRNA_Data(mRNAData_nativ3,"n3");
  sort.WriteRNA_Data(mRNAData_primaer,"p1");
  sort.WriteRNA_Data(mRNAData_primaer2,"p2");
  sort.WriteRNA_Data(mRNAData_primaer3,"p3");
  sort.WriteRNA_Data(mRNAData_oben,"o1");
  sort.WriteRNA_Data(mRNAData_oben2,"o2");
  sort.WriteRNA_Data(mRNAData_oben3,"o3");
  sort.WriteRNA_Data(mRNAData_unten,"u1");
  sort.WriteRNA_Data(mRNAData_unten2,"u2");
  sort.WriteRNA_Data(mRNAData_unten3,"u3");
}

void PrintHeader()
{
  std::cout << "------------------------------------------------------------------------" << std::endl;
  std::cout << "----------------- Welcome to the awesome plot creator! -----------------" << std::endl;
  std::cout << "------------------------------------------------------------------------" << std::endl;
}

void PrintQuestion()
{
  std::cout << "choose option: ('load' to load data / 'plot' to create plots / 'exit' to exit)" << std::endl;
}

main()
{ 
  auto mRNAs = LoadmRNAs();
  PrintHeader();
  bool loop = true;
  while (loop)
  {
    PrintQuestion();
    std::string answer;
    std::cin >> answer;
    if (answer == "load")
    {
      CreateDataSet(mRNAs);
    }
    else if (answer == "plot")
    {
      pyplot pp(mRNAs);
      pp.plot();
    }
    else if (answer == "exit")
    {
      break;
    }
    else
    {
      std::cerr << "Error: \"" << answer << "\" is not an option" << std::endl;
    }
  }
}