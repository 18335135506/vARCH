/* 
 * File:   asm_function.cpp
 * Author: ben
 * 
 * Created on 5 agosto 2010, 21.13
 */

#include "asm-function.h"

void
asm_function::finalize()
{
  DebugPrintf(("- Adding stmts and locals to function: %s -\n", name.c_str()));
  for(size_t index = 0; index < stmts.size(); index++) {
    asm_statement * stmt = stmts[index];

    stmt->offset = tempLocalOffset;
    tempLocalOffset += stmt->getSize();

    if (stmt->getType() == ASM_INSTRUCTION_STATEMENT) {
      asm_instruction_statement * istmt = (asm_instruction_statement *) stmt;
      for (size_t argNum = 0; argNum < istmt->args.size(); argNum++) {
        if (istmt->args[argNum]->getType() == ASM_LABEL_ARG) {
          ArgLabelRecord * tempRecord = new ArgLabelRecord();
          tempRecord->arg = (asm_label_arg *)istmt->args[argNum];
          tempRecord->parent = istmt;
          refs.push_back(tempRecord);
        }
      }
    } else {
      checkLabel(stmt);
    }
  }
  for(size_t i = 0; i < locals.size(); i++) {
    asm_data_statement * stmt = locals[i];

    stmt->offset = tempLocalOffset;
    tempLocalOffset += stmt->getSize();
    checkLabel(stmt);
  }
  DebugPrintf(("- Terminated: Adding stmts and locals -\n\n"));
}

bool
asm_function::checkInstructions() const
{
  bool error = false;
  for(deque<asm_statement *>::const_iterator stmt_it = this->stmts.begin();
      stmt_it != stmts.end(); stmt_it++)
  {
    const asm_statement * stmt = *stmt_it;
    if (stmt->getType() == ASM_INSTRUCTION_STATEMENT) {
      try {
        ((const asm_instruction_statement *)stmt)->checkArgs();
      } catch (const WrongArgumentException & e) {
        fprintf(stderr, "ERROR: in instruction!\n%s\n", e.what());
        error = true;
      }
    }
  }
  return error;
}

bool
asm_function::ensureNoTemps() const
{
  bool error = false;
  for(deque<asm_statement *>::const_iterator stmt_it = this->stmts.begin();
      stmt_it != stmts.end(); stmt_it++)
  {
    const asm_statement * stmt = *stmt_it;
    if (stmt->getType() == ASM_INSTRUCTION_STATEMENT) {
      try {
        ((const asm_instruction_statement *)stmt)->ensureNoTemps();
      } catch (const WrongArgumentException & e) {
        fprintf(stderr, "ERROR: in instruction!\n%s\n", e.what());
        error = true;
      }
    }
  }
  return error;
}

inline void
asm_function::checkLabel(asm_statement * stmt)
{
  if (stmt->getType() == ASM_LABEL_STATEMENT) {
    DebugPrintf(("Found local label: %s in function %s!\n",
           ((asm_label_statement *)stmt)->label.c_str(), name.c_str()));
    localSymbols.addLabel((asm_label_statement *)stmt);
  }
}
