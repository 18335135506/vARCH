/* 
 * File:   Cpu.h
 * Author: ben
 *
 * Created on 19 agosto 2009, 15.51
 */

#ifndef _CPU_H
#define	_CPU_H

#include "Mmu.h"
#include "Component.h"
#include "Chipset.h"
#include "../include/exceptions.h"

#define NUM_REGS 8

#define F_CARRY     (1 << 0)
#define F_OVERFLOW  (1 << 1)
#define F_ZERO      (1 << 2)
#define F_NEGATIVE  (1 << 3)
#define F_EXTEND    (1 << 4)
#define F_INT_MASK  (1 << 5)
#define F_SVISOR    (1 << 6)
#define F_TRACE     (1 << 7)

class Chipset; /* just a class declaration */

class Cpu : public Component {
public:
  Cpu(Chipset&, Mmu&);
  virtual ~Cpu();

  void init();

  void dumpRegistersAndMemory() const;

  int coreStep();
  
private:

  /** The actual flags of the cpu */
  int flags;

  /** link to the chipset */
  Chipset& chipset;

  /** the MMU */
  Mmu& memoryController;

  /** The data registers */
  int regsData[NUM_REGS];

//  /** The addresses registers */
//  int regsAddr[NUM_REGS];

  /** The program counter */
  int progCounter;

  int istructsOneArg(const int& istr, int& newFlags)
            throw(WrongIstructionException);
  int istructsZeroArg(const int& istr, int& newFlags)
            throw(WrongIstructionException);
  int istructsTwoArg(const int& istr, int& newFlags)
            throw(WrongIstructionException);
  int istructsThreeArg(const int& istr, int& newFlags)
            throw(WrongIstructionException);

  int loadArg(const int& arg,const int& typeArg)
            throw(WrongArgumentException);
  void storeArg(const int& arg, const int& typeArg, int value)
            throw(WrongArgumentException);
  
  void resetFlags(int& _flags) {
    _flags -= _flags & ( F_ZERO + F_CARRY + F_NEGATIVE + F_OVERFLOW );
  }
  void resetRegs() { for( int i = 0; i < NUM_REGS; i++) regsData[i] = 0; }
};

#endif	/* _CPU_H */

