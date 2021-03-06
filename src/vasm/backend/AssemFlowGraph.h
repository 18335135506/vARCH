/*
 * AssemFlowGraph.h
 *
 *  Created on: 19/lug/2011
 *      Author: ben
 */

#ifndef ASSEMFLOWGRAPH_H_
#define ASSEMFLOWGRAPH_H_

#include "../asm-function.h"
#include "../algorithms/FlowGraph.h"
#include "../algorithms/TempsMap.h"

class AssemFlowGraph : public FlowGraph<asm_statement *> {
  typedef std::map<asm_statement *, const NodeFlowGraph<asm_statement *> * > StmtToNode;

  StmtToNode backReference;

  TempsMap & tempsMap;

  std::string buildStmtLabel(const asm_statement * const stmt, const uint32_t & progr)
    const;

  void _addNodesToGraph(asm_function & function);
  void _createArcs(const TableOfSymbols & functionSymbols);
  void _findUsesDefines();

  void _addToSet(UIDMultiSetType & nodeSet, const uint32_t &shiftedUID);

  bool _moveInstr(const std::vector<asm_arg *> & args, UIDMultiSetType & nodeUses,
      UIDMultiSetType & nodeDefs);
  bool _argIsDefined(const int & instruction, const size_t & argNum,
      const TypeOfArgument & argType, const ModifierOfArgument & argMod) const;

public:
  AssemFlowGraph(TempsMap & _tm) : tempsMap(_tm) { }

  void populateGraph(asm_function & function);
  void checkTempsUsedUndefined(const asm_function & func,
      const LiveMap<asm_statement *> & lm) const;

  void applySelectedRegisters(const AssignedRegs & regs);
  void applySelectedRegisters(const AssignedRegs & regs,
      const AliasMap & aliases);

  virtual void clear();
};

#endif /* ASSEMFLOWGRAPH_H_ */
