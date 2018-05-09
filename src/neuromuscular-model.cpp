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

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include "muscle.hpp"

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{0};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq")) {
    std::cerr << argv[0] << " Neuromuscular model." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --freq=<Frequency> --cid=<od4 session> [--verbose]" << std::endl;
    std::cerr << "Example: " << argv[0] << " --freq=10 --cid=111" << std::endl;
    retCode = 1;
  } else {
    // bool const VERBOSE{commandlineArguments.count("verbose") != 0};
    uint16_t const CID = std::stoi(commandlineArguments["cid"]);
    float const FREQ = std::stof(commandlineArguments["freq"]);

    cluon::OD4Session od4{CID};

    cluon::data::TimeStamp t0 = cluon::time::now();
    auto atFrequency{[&t0]() -> bool
      {
        cluon::data::TimeStamp now = cluon::time::now();
        double time = ((double) now.seconds() + (double) now.microseconds()/1000000.0 - (double) t0.seconds() + t0.microseconds()/1000000.0);
        (void) time;
        std::cout << "Seconds: " << (float) ((double) now.seconds() + (double) now.microseconds()/1000000.0 - ((double) t0.seconds() + (double) t0.microseconds()/1000000.0)) << std::endl;
        return true;
      }};

    od4.timeTrigger(FREQ, atFrequency);
  }
  return retCode;
}
