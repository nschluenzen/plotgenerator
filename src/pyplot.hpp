#ifndef PYPLOT_HPP
#define PYPLOT_HPP
#include <vector>
#include <string>
class pyplot
{
public:
  pyplot(std::vector<std::string> _mRNAs, std::vector<std::string> _LegendPositions);
  void plot();
private:
  void CreatePlotFile();
  void printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument);
  void printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument, std::string argument);
  std::vector<int> ParseSelectionString(std::string SelectionString);
  void setLegendPositions();
  std::vector<std::string> ChooseIfNeeded(const std::vector<std::string> &PossibleChoices, std::string question, const std::vector<std::string> &Defaults, int RequiredNumberOfArguments);
  std::vector<std::string> mRNAs;
  std::vector<std::string> LegendPositions;
  std::vector<std::string> LegendAxPositions;
  std::vector<int> LegendAxs;
};

#endif // PYPLOT_HPP
