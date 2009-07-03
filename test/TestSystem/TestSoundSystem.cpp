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
//
// -----------------------------------------------------------------------

#include "TestSystem/TestSoundSystem.hpp"

// -----------------------------------------------------------------------
// TestSoundSystem
// -----------------------------------------------------------------------

TestSoundSystem::TestSoundSystem(System& system)
    : SoundSystem(system),
      sound_system_log_("TestSoundSystem") {
}

// -----------------------------------------------------------------------

TestSoundSystem::~TestSoundSystem() {}

// -----------------------------------------------------------------------

int TestSoundSystem::bgmStatus() const {
  sound_system_log_.recordFunction("bgm_status");
  return 0;
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmPlay(const std::string& bgm_name, bool loop) {
  sound_system_log_.recordFunction("bgm_play", bgm_name, loop);
  bgm_name_ = bgm_name;
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmPlay(const std::string& bgm_name,
                              bool loop, int fade_in_ms) {
  sound_system_log_.recordFunction("bgm_play", bgm_name, loop, fade_in_ms);
  bgm_name_ = bgm_name;
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmPlay(const std::string& bgm_name, bool loop,
                              int fade_in_ms, int fade_out_ms) {
  sound_system_log_.recordFunction("bgm_play", bgm_name, loop, fade_in_ms,
                                   fade_out_ms);
  bgm_name_ = bgm_name;
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmStop() {
  sound_system_log_.recordFunction("bgm_stop");
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmPause() {
  sound_system_log_.recordFunction("bgm_pause");
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmUnPause() {
  sound_system_log_.recordFunction("bgm_un_pause");
}

// -----------------------------------------------------------------------

void TestSoundSystem::bgmFadeOut(int fade_out_ms) {
  sound_system_log_.recordFunction("bgm_fade_out", fade_out_ms);
}

// -----------------------------------------------------------------------

std::string TestSoundSystem::bgmName() const {
  sound_system_log_.recordFunction("bgm_name");
  return bgm_name_;
}

// -----------------------------------------------------------------------

bool TestSoundSystem::bgmLooping() const {
  sound_system_log_.recordFunction("bgm_looping");
  return false;
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavPlay(const std::string& wav_file, bool loop) {
  sound_system_log_.recordFunction("wav_play", wav_file, loop);
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavPlay(const std::string& wav_file, bool loop,
                              const int channel) {
  sound_system_log_.recordFunction("wav_play", wav_file, loop, channel);
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavPlay(const std::string& wav_file, bool loop,
                              const int channel, const int fadein_ms) {
  sound_system_log_.recordFunction("wav_play", wav_file, loop, channel,
                                   fadein_ms);
}

// -----------------------------------------------------------------------

bool TestSoundSystem::wavPlaying(const int channel) {
  sound_system_log_.recordFunction("wav_playing", channel);

  // TODO: Might want to do something about this eventually.
  return false;
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavStop(const int channel) {
  sound_system_log_.recordFunction("wav_stop", channel);
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavStopAll() {
  sound_system_log_.recordFunction("wav_stop_all");
}

// -----------------------------------------------------------------------

void TestSoundSystem::wavFadeOut(const int channel, const int fadetime) {
  sound_system_log_.recordFunction("wav_fade_out", channel, fadetime);
}

// -----------------------------------------------------------------------

void TestSoundSystem::playSe(const int se_num) {
  sound_system_log_.recordFunction("play_se", se_num);
}

// -----------------------------------------------------------------------

bool TestSoundSystem::koePlaying() const {
  sound_system_log_.recordFunction("koePlaying");

  return false;
}

// -----------------------------------------------------------------------

void TestSoundSystem::koeStop() {
  sound_system_log_.recordFunction("koeStop");
}

// -----------------------------------------------------------------------

void TestSoundSystem::koePlayImpl(int id) {
  sound_system_log_.recordFunction("koePlayImpl", id);
}
