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
void ARCompactAsmPrinter::printOperand(const MachineInstr *MI, unsigned opNum,
    raw_ostream &O, const char* Modifier) {
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
      O << "@" << *MO.getMBB()->getSymbol();
      return;

    // A global address.
    case MachineOperand::MO_GlobalAddress:
      O << "@" << *Mang->getSymbol(MO.getGlobal());

      // Deal with any offsets.
      if (MO.getOffset() > 0) {
        O << "+" << MO.getOffset();
      } else if (MO.getOffset() < 0) {
        O << "-" << MO.getOffset();
      }

      break;

    // An external symbol.
    case MachineOperand::MO_ExternalSymbol:
      O << "@" << MO.getSymbolName();
      break;

    // A comment.
    case MachineOperand::MO_Metadata:
      // EOL comments need to be prefixed with a space.
      if (MI->getNumOperands() > 1) {
        O << " ";
      }
      O << "; " << cast<MDString>(MO.getMetadata()->getOperand(0))->getString();
      break;

    default:
      llvm_unreachable("Unknown operand type!");
  }
}

// Prints a memory operand, such as [r1,4].
void ARCompactAsmPrinter::printMemOperand(const MachineInstr *MI,
    unsigned opNum, raw_ostream &O, const char *Modifier) {
  O << "[";
  printOperand(MI, opNum, O);

  if (MI->getNumOperands() > 1) {
    MachineOperand MO = MI->getOperand(opNum + 1);
    // Only print the second part of a two-operand memory address if it is
    // non-zero; i.e. print [r1], not [r1,0].
    if (MO.getType() != MachineOperand::MO_Immediate || MO.getImm() != 0) {
      O << ",";
      printOperand(MI, opNum + 1, O);
    }
  }

  O << "]";
}

// Prints a condition code, such as "eq".
void ARCompactAsmPrinter::printCCOperand(const MachineInstr *MI, unsigned OpNum,
    raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNum).getImm();
  switch (CC) {
    case ARCCC::COND_AL:
      O << "al";
      break;
    case ARCCC::COND_EQ:
      O << "eq";
      break;
    case ARCCC::COND_NE:
      O << "ne";
      break;
    case ARCCC::COND_P:
      O << "p";
      break;
    case ARCCC::COND_N:
      O << "n";
      break;
    case ARCCC::COND_LO:
      O << "lo";
      break;
    case ARCCC::COND_HS:
      O << "hs";
      break;
    case ARCCC::COND_V:
      O << "v";
      break;
    case ARCCC::COND_NV:
      O << "nv";
      break;
    case ARCCC::COND_GT:
      O << "gt";
      break;
    case ARCCC::COND_GE:
      O << "ge";
      break;
    case ARCCC::COND_LT:
      O << "lt";
      break;
    case ARCCC::COND_LE:
      O << "le";
      break;
    case ARCCC::COND_HI:
      O << "hi";
      break;
    case ARCCC::COND_LS:
      O << "ls";
      break;
    case ARCCC::COND_PNZ:
      O << "pnz";
      break;
    default:
      assert(0 && "Unsupported CC code");
      break;
  }
}

void ARCompactAsmPrinter::printPredicateOperand(const MachineInstr *MI,
    unsigned OpNum, raw_ostream &O) {
  ARCCC::CondCodes CC = (ARCCC::CondCodes) MI->getOperand(OpNum).getImm();
  // AL is the default, do not print it.
  if (CC != ARCCC::COND_AL) {
    O << "." << ARCCCToString(CC);
  }
}

// Static initialization.
extern "C" void LLVMInitializeARCompactAsmPrinter() {
  RegisterAsmPrinter<ARCompactAsmPrinter> X(TheARCompactEncoreTarget);
}
