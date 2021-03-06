/* 
 * File:   vNewAsm.cpp
 * Author: ben
 *
 * Created on 7 agosto 2010, 0.29
 */

#include <cstdlib>
#include "AsmArgs.h"
#include "asm-program.h"
#include "IR_Low_parser.h"
#include "backend/Backend.h"

void
printAssembler(const asm_program & program);

void
printDefines(const AsmPreprocessor & defs);

/*
 * 
 */
int
main(int argc, char** argv)
{
  AsmArgs args(argc, argv);
  AsmPreprocessor defines;
  defines.addDefine(std::string("VASM_VERSION"), VERSION);

  try {
    args.parse();
  } catch (const WrongArgumentException & e) {
    if (e.getMessage().empty()) {
      args.printHelp();
      return EXIT_SUCCESS;
    } else {
      fprintf(stderr, "%s\n", e.what());
      args.printHelp();
      return EXIT_FAILURE;
    }
  }
  setIncludeDirs(&args.getIncludeDirs());

  try {
    Backend backend(args);

    for(const std::string & filename : args.getSrcInputNames()) {
      if (!openFirstFile(filename.c_str()))
      {
        fprintf(stderr, "I couldn't open the ASM file to process: %s\n",
                filename.c_str());
        return (EXIT_FAILURE);
      }

      ASTL_Tree ast_tree;

      int32_t res = yyparse(ast_tree, defines);
      if (res) {
        fprintf(stderr, "An error may have occurred, code: %3d\n", res);
        throw BasicException("Error parsing\n");
      }
      if (args.getOnlyValidate()) {
        ast_tree.printTree();
      } else {
        backend.sourceAST(ast_tree);
      }
    }

    if (!args.getOnlyValidate()) {
      backend.emit();
    }

#ifdef DEBUG
      printAssembler(backend.getProgram());
#endif

    cleanParser();
    printDefines(defines);

  } catch (const BasicException & e) {
    fprintf(stderr, "Error: %s\n", e.what());
    return (EXIT_FAILURE);
  }
}

void
printAssembler(const asm_program & program)
{
#ifdef DEBUG
  DebugPrintf(("-- Dumping Schematic Parsed Code --\n"));
  for(const asm_function * func : program.functions)
  {
    DebugPrintf(("Line: %03d Function: %s\n", func->position.first_line,
                  func->name.c_str()));
    for(const asm_statement * stmt : func->stmts)
    {
      DebugPrintf((" Line: %03d %s\n", stmt->position.first_line,
                    stmt->toString().c_str()));
    }
    for(asm_data_statement * stmt : func->stackLocals)
    {
      DebugPrintf((" Line: %03d Local: %s\n",
          stmt->position.first_line, stmt->toString().c_str()));
    }
  }
  for(asm_data_statement * stmt : program.shared_vars)
  {
    DebugPrintf(("Line: %03d Shared Variable: %s\n",
        stmt->position.first_line, stmt->toString().c_str()));
  }
  for(asm_data_statement * stmt : program.constants)
  {
    DebugPrintf(("Line: %03d Constant: %s\n",
        stmt->position.first_line, stmt->toString().c_str()));
  }
  DebugPrintf(("-- Terminated Dumping Parsed Code --\n\n"));
#endif
}

void
printDefines(const AsmPreprocessor & defs)
{
  const DefineType & d = defs.getDefines();
  DebugPrintf(("Num of Defines %lu\n", d.size()));
  for(const auto entry : d) {
    std::string argsDescr;
    for(const std::string param : entry.second.parameters) {
      argsDescr += " ";
      argsDescr += param;
    }
    DebugPrintf(("- Name: \"%s\"\n", entry.first.c_str() ));
    DebugPrintf(("  Params: \"%s\"\n", argsDescr.c_str() ));
    DebugPrintf(("  Definition: \"%s\"\n", entry.second.content.c_str() ));
  }
}

