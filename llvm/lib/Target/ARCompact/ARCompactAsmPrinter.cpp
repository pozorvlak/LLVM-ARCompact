//===----- ARCompactAsmPrinter.cpp - ARCompact LLVM assembly writer -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to ARCOMPACT assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"

#include "ARCompactAsmPrinter.h"

using namespace llvm;

// Pull in the printInstruction method, etc.
#include "ARCompactGenAsmWriter.inc"

// Prints a basic operand, such as a register or an immediate.
void ARCompactAsmPrinter::printOperand(const MachineInstr *MI, int opNum,
    raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand (opNum);
  switch (MO.getType()) {
    // A register.
    case MachineOperand::MO_Register:
      O << LowercaseString(getRegisterName(MO.getReg()));
      break;

    // An immediate.
    case MachineOperand::MO_Immediate:
      O << (int) MO.getImm();
      break;

    // An entire basic block.
    case MachineOperand::MO_MachineBasicBlock:
      O << *MO.getMBB()->getSymbol();
      return;

    // A global address.
    case MachineOperand::MO_GlobalAddress:
      O << *Mang->getSymbol(MO.getGlobal());
      break;

    // An external symbol.
    case MachineOperand::MO_ExternalSymbol:
      O << MO.getSymbolName();
      break;

    default:
      llvm_unreachable("Unknown operand type!");
  }
}


// Static initialization.
extern "C" void LLVMInitializeARCompactAsmPrinter() { 
  RegisterAsmPrinter<ARCompactAsmPrinter> X(TheARCompactEncoreTarget);
}
