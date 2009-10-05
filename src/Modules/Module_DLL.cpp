// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2008 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// -----------------------------------------------------------------------

#include "Precompiled.hpp"

// -----------------------------------------------------------------------

#include "Modules/Module_DLL.hpp"

#include "MachineBase/GeneralOperations.hpp"
#include "MachineBase/RLMachine.hpp"
#include "MachineBase/RLOperation.hpp"
#include "MachineBase/RLOperation/DefaultValue.hpp"

#include <string>

using namespace libReallive;

// -----------------------------------------------------------------------

struct Jmp_LoadDLL : public RLOp_Void_2<IntConstant_T, StrConstant_T> {
  void operator()(RLMachine& machine, int slot, string name) {
    machine.loadDLL(slot, name);
  }
};

// -----------------------------------------------------------------------

struct Jmp_CallDLL : public RLOp_Store_6<
  IntConstant_T,
  DefaultIntValue_T<0>, DefaultIntValue_T<0>, DefaultIntValue_T<0>,
  DefaultIntValue_T<0>, DefaultIntValue_T<0> > {
  int operator()(RLMachine& machine, int index, int one, int two, int three,
                 int four, int five) {
    return machine.callDLL(index, one, two, three, four, five);
  }
};

// -----------------------------------------------------------------------

DLLModule::DLLModule()
  : RLModule("DLL", 2, 1) {
  addOpcode(10, 0, "LoadDLL", new Jmp_LoadDLL);
  addOpcode(11, 0, "UnloadDLL", callFunction(&RLMachine::unloadDLL));

  addOpcode(12, 0, "CallDLL", new Jmp_CallDLL);
  addOpcode(12, 1, "CallDLL", new Jmp_CallDLL);
  addOpcode(12, 2, "CallDLL", new Jmp_CallDLL);
  addOpcode(12, 3, "CallDLL", new Jmp_CallDLL);
  addOpcode(12, 4, "CallDLL", new Jmp_CallDLL);
  addOpcode(12, 5, "CallDLL", new Jmp_CallDLL);
}
