/*
 * Disassembler.h
 *
 *  Created on: 02/ago/2011
 *      Author: ben
 */

#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_

#include "../asm-program.h"

class Disassembler {
  void printArg(const int32_t & typeArg, const int64_t & arg);
  const int64_t fetchArg(const int32_t & typeArg, Bloat::const_iterator & codeIt,
      const Bloat::const_iterator & endIt);
public:
//  Disassembler();

  void disassembleAndPrint(const Bloat & bytecode);
  void disassembleProgram(asm_program & prog);
};

#endif /* DISASSEMBLER_H_ */
