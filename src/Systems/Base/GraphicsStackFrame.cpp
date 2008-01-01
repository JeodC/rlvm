// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2007 Elliot Glaysher
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
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
//  
// -----------------------------------------------------------------------

#include "Precompiled.hpp"

// -----------------------------------------------------------------------

#include "Systems/Base/GraphicsStackFrame.hpp"

// -----------------------------------------------------------------------

GraphicsStackFrame::GraphicsStackFrame() 
  : m_commandName(""), m_hasFilename(false), m_hasSourceDC(false),
    m_hasSourceCoordinates(false), m_hasTargetDC(false),
    m_hasTargetCoordinates(false), m_hasRGB(false), m_hasOpacity(false),
    m_hasMask(false)
{}

// -----------------------------------------------------------------------

GraphicsStackFrame::GraphicsStackFrame(const std::string& name)
  : m_commandName(name), m_hasFilename(false), m_hasSourceDC(false),
    m_hasSourceCoordinates(false), m_hasTargetDC(false),
    m_hasTargetCoordinates(false), m_hasRGB(false), m_hasOpacity(false),
    m_hasMask(false)
{}

// -----------------------------------------------------------------------

GraphicsStackFrame::~GraphicsStackFrame() {}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setFilename(const std::string& filename) 
{
  m_hasFilename = true;
  m_fileName = filename;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setSourceDC(int in)
{
  m_hasSourceDC = true;
  m_sourceDC = in;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setSourceCoordinates(
  int x1, int y1, int x2, int y2)
{
  m_hasSourceCoordinates = true;
  m_sourceX = x1;
  m_sourceY = y1;
  m_sourceX2 = x2;
  m_sourceY2 = y2;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setTargetDC(int in)
{
  m_hasTargetDC = true;
  m_targetDC = in;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setTargetCoordinates(int x1, int y1)
{
  m_hasTargetCoordinates = true;
  m_targetX = x1;
  m_targetY = y1;
  m_targetX2 = -1;
  m_targetY2 = -1;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setTargetCoordinates(
  int x1, int y1, int x2, int y2)
{
  m_hasTargetCoordinates = true;
  m_targetX = x1;
  m_targetY = y1;
  m_targetX2 = x2;
  m_targetY2 = y2;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setRGB(int r, int g, int b)
{
  m_hasRGB = true;
  m_r = r;
  m_g = g; 
  m_b = b;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setOpacity(int in)
{
  m_hasOpacity = true;
  m_opacity = in;
  return *this;
}

// -----------------------------------------------------------------------

GraphicsStackFrame& GraphicsStackFrame::setMask(bool in)
{
  m_hasMask = true;
  m_mask = in;
  return *this;
}