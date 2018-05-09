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
#include <Eigen/Eigen>

#include "body.hpp"


Body::Body()
    : m_state{}
    , m_forceDisturbance{}
    , m_phiReaching{}
    , m_muscleA{}
    , m_muscleB{}

{
  m_state.setZero();
}
Body::~Body()
{}

void Body::step(double const &a_dt)
{
  Eigen::Vector3d const deltaState = Eigen::Vector3d(m_state[1],m_forceDisturbance,0) * a_dt; 
  m_state = m_state + deltaState;
}

void Body::setForceDisturbance(double const &a_val)
{
  // Temp for checking integration step
  m_forceDisturbance = a_val;
  m_state[2] = a_val;
}

void Body::setPhiReaching(double const &a_val)
{
  m_phiReaching = a_val;
}

std::string Body::toString()
{
  Eigen::IOFormat CleanFmt(0, 0, " ", " ", "", "");
  std::stringstream ss;
  ss << "Force disturbance: " << m_forceDisturbance << std::endl;
  ss << "Phi reaching: " << m_phiReaching << std::endl;
  ss << "Muscle A: " << std::endl <<  "\t" << m_muscleA.toString() << std::endl;
  ss << "Muscle B: " << std::endl <<  "\t" << m_muscleB.toString() << std::endl;
  ss << "Body state (position, velocity, acceleration): " << m_state.format(CleanFmt) << std::endl;
  return ss.str();
}