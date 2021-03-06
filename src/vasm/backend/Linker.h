/*
 * Linker.h
 *
 *  Created on: 09/feb/2013
 *      Author: ben
 */

#ifndef LINKER_H_
#define LINKER_H_

#include "../asm-program.h"

class Linker {
  asm_program & program;

  const AsmArgs & args;

  void checkNoPrototypes() const;

  void moveMainToTop();
  void rebuildOffsets(const bool & is_obj = false);
  void rebuildFunctionOffsets(asm_function & func);

  void exposeGlobalLabels();
  void updateFunctionLabels();

  void assignValuesToLocalLabels();
  void assignValuesToLabels();
public:
  Linker(asm_program & _prog, const AsmArgs & _args)
    : program(_prog), args(_args)
  { }

  void prelink();
  void link();
};

#endif /* LINKER_H_ */
