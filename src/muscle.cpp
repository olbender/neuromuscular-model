/*
 * Copyright (C) 2018 Ola Benderius, Björnborg Nguyen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sstream>
#include <random>

#include "muscle.hpp"

Muscle::Muscle()
  : m_forceDisturbance{}
  , m_phiDot{}
  , m_fibers{}
  , m_numFibers{100}
  , m_tension{0}
  , m_periods{0.010, 0.020, 0.070}
  , m_lastStimulii{cluon::time::now()}
{
  m_fibers.setLinSpaced(m_numFibers, 0, 1);

}
Muscle::~Muscle()
{}

void Muscle::setForceDisturbance(double const &a_val)
{
  m_forceDisturbance = a_val;
}

void Muscle::setPhiDot(double const &a_val)
{
  m_phiDot = a_val;
}

double Muscle::twitch(double const &a_percent)
{
  uint32_t numFibers = static_cast<uint32_t>((a_percent * m_fibers.size()));
  double combinedMuscleTwitchForce = m_fibers.head(numFibers).sum();
  return combinedMuscleTwitchForce;
}

void Muscle::Stimulate()
{
  m_lastStimulii = cluon::time::now();
}

double Muscle::GetForce()
{
  return 0.0;
}

float Muscle::GetForcef()
{
  return static_cast<float>(GetForce());
}

std::string Muscle::toString()
{
  std::stringstream ss;
  ss << "Force disturbance: " << m_forceDisturbance << std::endl;
  ss << "\tPhi dot: " << m_phiDot << std::endl;
  return ss.str();
}
