/* 
 * File:   Chipset.cpp
 * Author: ben
 * 
 * Created on 20 agosto 2009, 14.51
 */

#include <unistd.h>
#include <stdio.h>

#include "Chipset.h"
#include "../include/StdIstructions.h"

Chipset::Chipset(const int& _maxMem, int * _mainMem)
    : cpu(* (new Cpu(*this,*(new Mmu(_maxMem,_mainMem))))), mainMem(_mainMem)
      , maxMem(_maxMem) {

  initMem();
}

//Chipset::Chipset(const Chipset& orig) { }

Chipset::~Chipset() { }

const int
Chipset::bios[] = {
  0, 0,
  HALT, 0, 0 };

void
Chipset::initMem() {
  int i = 0;
  for (i = 0; !(bios[i-1] == HALT && (bios[i] == 0 || bios[i+1] == 0)); i++) {
    mainMem[i] = bios[i];
  }
  for (; i < maxMem; i++) {
    mainMem[i] = 0;
  }
}

void
Chipset::startClock() {
  printf("Starting execution\n");

  int result = 0;
  do {
    result = cpu.coreStep();
    usleep(100);

    if (result == REBOOT) {
      initMem();
      cpu.init();
    }
  } while (result != HALT);

  printf("Exiting\n");
}


void
Chipset::addComponent(const Component& comp) {
  components.push_back(comp);
}

//void
//Chipset::addCpu(Cpu & _cpu) {
//  cpu = _cpu;
//}

const Cpu&
Chipset::getCpu(int num) const {
  /* for now num is just an API placeholder */
  return cpu;
}

void
Chipset::singlePutToComponent(const int& numComp, const int& signal) {
  components[numComp].put(signal);
}

int
Chipset::singleGetFromComponent(const int& numComp) {
  components[numComp].get();
}
