/* 
 * File:   asm_program.h
 * Author: ben
 *
 * Created on 5 agosto 2010, 21.05
 */

#ifndef ASM_PROGRAM_H
#define	ASM_PROGRAM_H

#include "asm-classes.h"
#include "asm-function.h"
#include "exceptions.h"

#include <deque>

struct asm_program {
  int tempOffset;

  deque<asm_function *> functions;
  vector<asm_data_statement *> globals;

  TableOfSymbols globalSymbols;

public:
  asm_program(list<asm_function *> * _funcs,
              list<asm_data_statement *> * _globals);

  void addFunction(asm_function * _func);
  void addFunctions(list<asm_function *> * _funcs);

  void addGlobal(asm_data_statement * _global);
  void addGlobals(list<asm_data_statement *> * _globals);

  int getFunciontsTotalSize() {
    int totSize = 0;
    for(size_t i = 0; i < functions.size(); i++) {
      totSize += functions[i]->getSize();
    }
    return totSize;
  }

  void addFunctionLabelsToGlobals();
  void assignValuesToLabels();
  void assemble(const string & outputName);
};

#endif	/* ASM_PROGRAM_H */

