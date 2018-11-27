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

#include <Eigen/Dense>
#include <string>
#include <vector>
#include "cluon-complete.hpp"

class Muscle{
 public:
  Muscle();
  ~Muscle();

  void setForceDisturbance(double const &);
  void setPhiDot(double const &);

  std::string toString();

  void Stimulate();
  double GetForce();
  float GetForcef();

 private:
  Muscle(const Muscle &) = delete;
  Muscle(Muscle &&) = delete;
  Muscle &operator=(const Muscle &) = delete;
  Muscle &operator=(Muscle &&) = delete;

  double m_forceDisturbance;
  double m_phiDot;

  Eigen::VectorXd m_fibers;
  uint32_t const m_numFibers;
  double m_tension;
  std::vector<double> const m_periods;
  cluon::data::TimeStamp m_lastStimulii;


  double twitch(double const &);
};