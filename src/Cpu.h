/* 
 * File:   Cpu.h
 * Author: ben
 *
 * Created on 19 agosto 2009, 15.51
 */

#ifndef _CPU_H
#define	_CPU_H

#include "Mmu.h"
#include "exceptions.h"
#include "CpuDefinitions.h"
#include "std_istructions.h"

class Chipset; /* just a class declaration */

class Cpu {
public:
  Cpu(Chipset&, Mmu&);

  void init();

  void dumpRegistersAndMemory() const;

  int coreStep();
  
  const uint32_t & getTimeDelay() const throw() { return timeDelay; }

private:

  uint32_t timeDelay;

  /** The actual flags of the cpu */
  int flags;

  /** link to the chipset */
  Chipset& chipset;

  /** the MMU */
  Mmu& memoryController;

  /** The raw_data registers */
  int regsData[NUM_REGS];

  /** The addresses registers */
  int regsAddr[NUM_REGS];

  struct StackPointers {
  private:
    uint32_t sSP; /* supervisor stack pointer */
    uint32_t uSP; /* user stack pointer */

    Cpu& cpu;
  public:
    StackPointers(Cpu& _c) : cpu(_c) { }
    
    void setStackPointer(const uint32_t& newSP) {
      if (cpu.flags & F_SVISOR) sSP = newSP; else uSP = newSP;
    }
    void setUStackPointer(const uint32_t& newSP) { uSP = newSP; }

    const uint32_t & getStackPointer() const throw() {
      return (cpu.flags & F_SVISOR) ? sSP : uSP;
    }
    const uint32_t & getUStackPointer() const throw() { return uSP; }

    void push(const int32_t& data);
    int32_t pop();

    void pushAllRegs();
    void popAllRegs();
  } sP;

  /** The program counter */
  uint32_t progCounter;

  //|//////////////////////|//
  //|  Functions           |//
  //|//////////////////////|//
  int32_t instructsOneArg(const int32_t& instr, int32_t& newFlags);
  int32_t instructsZeroArg(const int32_t& instr, int32_t& newFlags);
  int32_t instructsTwoArg(const int32_t& instr, int32_t& newFlags);
  int32_t instructsThreeArg(const int32_t& instr, int32_t& newFlags);

  /**
   * Temporary record for arguments processing
   */
  struct ArgRecord {
    uint8_t scale;
    uint8_t type;
    uint32_t raw_data;

    ArgRecord(const int32_t & packedType, const int32_t & data);
  };

  /* Arguments functions */
  uint32_t loadArg(int32_t & temp, const ArgRecord &argRecord);
  uint32_t storeArg(const int32_t & value, const ArgRecord & argRecord);

  /* Regs functions */
  int getReg(const int& arg);
  void setReg(const int& arg, const int& value);

  void resetRegs() throw() {
    for(size_t i = 0; i < NUM_REGS; i++) regsData[i] = regsAddr[i] = 0;
  }

  /* Flags functions */
  void resetFlags(int& _flags) throw() {
    _flags -= _flags & ( F_ZERO + F_CARRY + F_NEGATIVE + F_OVERFLOW );
  }
  void restoreFlags(const int& _flags) throw() { flags = _flags; }
  int clearFlags(int mask) throw() {
    int oldFlags = flags;
    flags -= flags & mask;
    return oldFlags;
  }
  int setFlags(int mask) throw() {
    int oldFlags = flags;
    flags |= mask;
    return oldFlags;
  }

  static int32_t fromMemorySpace(const DoubleWord & data, const uint8_t & scale);
  static void toMemorySpace(DoubleWord & data, const int32_t & value,
      const uint8_t & scale);

  static bool isAutoIncrDecrArg(const ArgRecord & arg);
};

#endif	/* _CPU_H */

