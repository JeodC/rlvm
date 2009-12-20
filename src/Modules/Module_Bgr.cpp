// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2009 Elliot Glaysher
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

#include "Modules/Module_Bgr.hpp"

#include "Effects/EffectFactory.hpp"
#include "Effects/Effect.hpp"
#include "MachineBase/RLMachine.hpp"
#include "MachineBase/RLOperation.hpp"
#include "MachineBase/RLOperation/Argc_T.hpp"
#include "MachineBase/RLOperation/Complex_T.hpp"
#include "MachineBase/RLOperation/Special_T.hpp"
#include "Modules/Module_Grp.hpp"
#include "Systems/Base/Colour.hpp"
#include "Systems/Base/GraphicsSystem.hpp"
#include "Systems/Base/Surface.hpp"
#include "Systems/Base/System.hpp"

#include <iostream>
#include <string>

using namespace std;

// Working theory of how this module works: The haikei module is one backing
// surface and (optionally) a HIK script. Games like AIR and the Maiden Halo
// demo use just the surface with a combination of bgrMulti and
// bgrLoadHaikei. OTOH, ALMA and planetarian use HIK scripts and the whole
// point of HIK scripts is to manipulate the backing surface on a timer that's
// divorced from the main interpreter loop.

namespace {

struct bgrLoadHaikei_blank : public RLOp_Void_1<IntConstant_T> {
  void operator()(RLMachine& machine, int sel) {
    GraphicsSystem& graphics = machine.system().graphics();
    graphics.setDefaultBgrName("");

    boost::shared_ptr<Surface> before =
      graphics.renderToSurfaceWithBg(graphics.getDC(0));
    graphics.getHaikei()->fill(RGBAColour::Clear());
    boost::shared_ptr<Surface> after =
      graphics.renderToSurfaceWithBg(graphics.getDC(0));

    LongOperation* effect =
        EffectFactory::buildFromSEL(machine, after, before, sel);
    machine.pushLongOperation(effect);
  }
};

struct bgrLoadHaikei_main : RLOp_Void_2<StrConstant_T, IntConstant_T> {
  void operator()(RLMachine& machine, string filename, int sel) {
    GraphicsSystem& graphics = machine.system().graphics();
    graphics.setDefaultBgrName(filename);

    // bgrLoadHaikei clears the stack.
    graphics.clearStack();

    boost::shared_ptr<Surface> before =
      graphics.renderToSurfaceWithBg(graphics.getDC(0));
    boost::shared_ptr<Surface> source(
        graphics.loadSurfaceFromFile(machine, filename));
    boost::shared_ptr<Surface> haikei = graphics.getHaikei();
    source->blitToSurface(*haikei,
                          source->rect(),
                          source->rect(),
                          255, true);
    boost::shared_ptr<Surface> after =
      graphics.renderToSurfaceWithBg(graphics.getDC(0));

    LongOperation* effect =
        EffectFactory::buildFromSEL(machine, after, before, sel);
    machine.pushLongOperation(effect);
  }
};

struct bgrLoadHaikei_wtf
    : RLOp_Void_4<StrConstant_T, IntConstant_T, IntConstant_T, IntConstant_T> {
  void operator()(RLMachine& machine, string filename, int sel, int a, int b) {
    cerr << "Filename: " << filename
         << "(a: " << a << ", b: " << b << ")" << endl;
    machine.system().graphics().setDefaultBgrName(filename);
  }
};

}  // namespace

// -----------------------------------------------------------------------

BgrModule::BgrModule()
    : RLModule("Bgr", 1, 40) {
  addOpcode(10, 0, "bgrLoadHaikei", new bgrLoadHaikei_blank);
  addOpcode(10, 1, "bgrLoadHaikei", new bgrLoadHaikei_main);
  addOpcode(10, 2, "bgrLoadHaikei", new bgrLoadHaikei_wtf);

  addUnsupportedOpcode(100, 0, "bgrMulti");
  addOpcode(100, 1, "bgrMulti", makeBgrMulti1());
}
