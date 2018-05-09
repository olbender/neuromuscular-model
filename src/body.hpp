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

#include "muscle.hpp"

class Body{
 public:
  Body();
  ~Body();

  void setForceDisturbance(double const &);
  void setPhiReaching(double const &);

 private:
  Body(const Body &) = delete;
  Body(Body &&) = delete;
  Body &operator=(const Body &) = delete;
  Body &operator=(Body &&) = delete;

  double m_forceDisturbance;
  double m_phiReaching;

  Muscle m_muscleA;
  Muscle m_muscleB;
};