#ifndef PYPLOT_HPP
#define PYPLOT_HPP
#include <vector>
#include <string>
class pyplot
{
public:
  pyplot(std::vector<std::string> _mRNAs);
  void plot();
private:
  void CreatePlotFile();
  void printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument);
  void printParserErrorDescription(std::exception& e, std::string file, int line, std::string functionArgument, std::string argument);
  std::vector<int> ParseSelectionString(std::string SelectionString);
  std::vector<std::string> ChooseIfNeeded(const std::vector<std::string> &PossibleChoices, std::string question, const std::vector<std::string> &Defaults, int RequiredNumberOfArguments);
  std::vector<std::string> mRNAs;
};

#endif // PYPLOT_HPP
