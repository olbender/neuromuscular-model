/*
 * Copyright (C) 2018 Ola Benderius
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

#include "muscle.hpp"

Muscle::Muscle()
  : m_forceDisturbance{}
  , m_phiDot{}
{}
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

std::string Muscle::toString()
{
  std::stringstream ss;
  ss << "Force disturbance: " << m_forceDisturbance << std::endl;
  ss << "\tPhi dot: " << m_phiDot << std::endl;
  return ss.str();
}