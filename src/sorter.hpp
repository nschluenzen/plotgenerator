#ifndef SORTER_HPP
#define SORTER_HPP
#include <vector>
#include <tuple>
#include <string>
class sorter
{
public:
  sorter(std::vector<std::string> _mRNAs);
  std::vector<std::pair<std::string,std::vector<double>>> getRNA_Data(std::string filename);
  void WriteRNA_Data(std::vector<std::pair<std::string,std::vector<double>>> mRNAData, std::string name);
private:
  std::vector<std::string> mRNAs;
};

#endif // SORTER_HPP
