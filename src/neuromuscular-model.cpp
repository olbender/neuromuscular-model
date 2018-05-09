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

#include <ncurses.h>

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include "body.hpp"

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{0};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq")) {
    std::cerr << argv[0] << " Neuromuscular model." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --freq=<Frequency> --cid=<od4 session> [--verbose]" << std::endl;
    std::cerr << "Example: " << argv[0] << " --freq=10 --cid=111" << std::endl;
    retCode = 1;
    return retCode;
  } 

  // bool const VERBOSE{commandlineArguments.count("verbose") != 0};
  uint16_t const CID = std::stoi(commandlineArguments["cid"]);
  double const FREQ = std::stod(commandlineArguments["freq"]);
  double const DT = 1 / FREQ;

  cluon::OD4Session od4{CID};
  Body body;

  initscr();

  cluon::data::TimeStamp t0 = cluon::time::now();
  auto atFrequency{[&t0, &body, &DT]() -> bool
    {
      cluon::data::TimeStamp now = cluon::time::now();
      double time = ((double) now.seconds() + (double) now.microseconds()/1000000.0 - (double) t0.seconds() + t0.microseconds()/1000000.0);

      if (time < 5) {
        body.setForceDisturbance(0.0);
      } else if (time < 10) {
        body.setForceDisturbance(1.0);

      } else if (time < 15) {
        body.setForceDisturbance(-1.0);
      } else {
        body.setForceDisturbance(0.0);
      }
      body.step(DT);

      mvprintw(1,0, ("Time: " + std::to_string(time)).c_str());
      mvprintw(2,0, body.toString().c_str());
      refresh();
      return true;
    }};

  od4.timeTrigger((float) FREQ, atFrequency);
  endwin();
  return retCode;
}
