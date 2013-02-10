/*
 * AsmArgs.h
 *
 *  Created on: 20/mar/2011
 *      Author: ben
 */

#ifndef ASMARGS_H_
#define ASMARGS_H_

#include "macros.h"
#include "exceptions.h"

#include <vector>
using namespace std;

class AsmArgs {
  int argc;
  char** argv;

  vector<string> inputFiles;
  string outputName;
  string debugSymbolsName;
  vector<string> includeDirs;

  uint32_t optimLevel;

  bool regAutoAlloc;
  bool regCoalesce;

  bool onlyCompile;
  bool onlyValidate;
  bool disassembleResult;
  bool omitFramePointer;
public:
  AsmArgs(int _argc, char** _argv)
    : argc(_argc), argv(_argv), optimLevel(2), regAutoAlloc(false)
    , regCoalesce(false), onlyCompile(false), onlyValidate(false)
    , disassembleResult(false), omitFramePointer(false)
  { }

  void parse() throw(WrongArgumentException);

  void printHelp() const throw();

  const vector<string> & getInputName() const throw() { return inputFiles; }
  const string &getOutputName() const throw() { return outputName; }
  const string &getDebugSymbolsName() const throw() { return debugSymbolsName; }
  const vector<string> &getIncludeDirs() const throw() { return includeDirs; }
  const uint32_t &getOptimizationLevel() const throw() { return optimLevel; }
  const bool &getRegAutoAlloc() const throw() { return regAutoAlloc; }
  const bool &getRegCoalesce() const throw() { return regCoalesce; }
  const bool &getOnlyCompile() const throw() { return onlyCompile; }
  const bool &getOnlyValidate() const throw() { return onlyValidate; }
  const bool &getDisassembleResult() const throw() { return disassembleResult; }
  const bool &getOmitFramePointer() const throw() { return omitFramePointer; }
};

#endif /* ASMARGS_H_ */
