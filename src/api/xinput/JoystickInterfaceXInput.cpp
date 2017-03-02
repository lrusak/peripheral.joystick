/*
 *      Copyright (C) 2014-2017 Garrett Brown
 *      Copyright (C) 2014-2017 Team Kodi
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "JoystickInterfaceXInput.h"
#include "JoystickXInput.h"
#include "XInputDLL.h"
#include "api/JoystickTypes.h"

#include <array>
#include <cstring>
#include <Xinput.h>

using namespace JOYSTICK;

#define MAX_JOYSTICKS 4

EJoystickInterface CJoystickInterfaceXInput::Type(void) const
{
  return EJoystickInterface::XINPUT;
}

bool CJoystickInterfaceXInput::Initialize(void)
{
  return CXInputDLL::Get().Load();
}

void CJoystickInterfaceXInput::Deinitialize(void)
{
  CXInputDLL::Get().Unload();
}

bool CJoystickInterfaceXInput::SupportsPowerOff(void) const
{
  return CXInputDLL::Get().SupportsPowerOff();
}

bool CJoystickInterfaceXInput::ScanForJoysticks(JoystickVector& joysticks)
{
  // No need to memset, only checking for controller existence
  XINPUT_STATE controllerState;
  XINPUT_STATE_EX controllerStateWithGuide;

  for (unsigned int i = 0; i < MAX_JOYSTICKS; i++)
  {
    if (CXInputDLL::Get().HasGuideButton())
    {
      if (!CXInputDLL::Get().GetStateWithGuide(i, controllerStateWithGuide))
        continue;
    }
    else
    {
      if (!CXInputDLL::Get().GetState(i, controllerState))
        continue;
    }

    joysticks.push_back(JoystickPtr(new CJoystickXInput(i)));
  }

  return true;
}
