/*
 * Copyright (C) 2019 Ola Benderius, Björnborg Nguyen
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

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include <cstdint>
#include <cmath>
#include <iostream>
#include <memory>
#include <iterator>

struct Muscle {
  std::vector<std::pair<double, double>> spindleToSpinePipe;
  double spindleToSpineIntegrator;
  double force;
  double velocity;
  double length;
};

/*
struct Reaching {
  double onset;
  double startLoad;
  double startPosition;
  double target;
};

struct StretchReflex {
  double onset;
  double 
*/

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{1};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ( (0 == commandlineArguments.count("cid")) ||
      (0 == commandlineArguments.count("freq")) )  {
    std::cerr << argv[0] << " steeres towards and aim point by using a "
      << "neurological model of two muscles." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> "
      << "--freq=<Frequency> [--verbose]" << std::endl;
  } else {
    bool const verbose{commandlineArguments.count("verbose") != 0};

    int32_t const freq{std::stoi(commandlineArguments["freq"])};
    double const dt{1.0 / freq};

    cluon::OD4Session od4{
      static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

    /*
    auto onObjectType{[&uncompleteFrameMutex, &uncompleteFrame](
        cluon::data::Envelope &&envelope)
    {
      opendlv::logic::perception::ObjectType msg = 
        cluon::extractMessage<opendlv::logic::perception::ObjectType>(
            std::move(envelope));

      uint32_t objectId = msg.objectId();
      {
        std::lock_guard<std::mutex> lock(uncompleteFrameMutex);

        if (uncompleteFrame.count(objectId)) {
          uncompleteFrame[objectId].type = msg.type();
          uncompleteFrame[objectId].collectedComponents++;
        } else {
          Object object;
          object.objectId = objectId;
          object.type = msg.type();
          object.collectedComponents = 1;
          uncompleteFrame[objectId] = object;
        }
      }
    }};
    */

    double const mass{1.0};
    double time{0.0};
    double load{0.0};
    double target{0.0};

    std::pair<Muscle, Muscle> muscles;

    auto atFrequency{[&od4, &dt, &mass, &time, &mass, &currentPosition,
      &currentVelocity, &currentMuscleAForce, &currentMuscleBForce,
      &currentLoad, &targetPosition, &spindleToSpinePipe, &spindleIntegrator,
      &verbose]() -> bool
      {

        double const force{currentMuscleAForce + currentMuscleBForce 
          + currentLoad};
        double const acceleration{force / mass};
        currentVelocity += acceleration * dt;
        currentPosition += currentVelocity * dt;
        if (currentPosition < 0.0 || currentPosition > 1.0) {
          currentVelocity = 0.0;
          currentPosition = (currentPosition < 0.0) ? 0.0 
            : (currentPosition > 1.0) ? 1.0 : currentPosition;
        }

        double const expectedVelocity{0.0};
        double const extraVelocity{currentVelocity - expectedVelocity};

        spindleToSpinePipe.push_pack({time, currentVelocity});

        time += dt;



        if (verbose) {

          uint32_t const muscleOffsetI{50};
          uint32_t const muscleOffsetJ{50};
          uint32_t const muscleHeight{400};
          uint32_t const muscleWidth{100};

          uint32_t const tenthWidth = 0.1 * muscleWidth;
          uint32_t const area = (muscleWidth * muscleHeight) / 16;
          uint32_t const centerI = muscleOffsetI + 0.5 * muscleWidth;
          uint32_t const rightI = muscleOffsetI + muscleWidth;
          uint32_t const lowerJ = muscleOffsetJ + muscleHeight;
          uint32_t const centerPointJ = static_cast<uint32_t>(muscleOffsetJ 
              + 0.25 * muscleHeight + currentPosition * 0.5 * muscleHeight);
          uint32_t const upperPointJ = static_cast<uint32_t>(muscleOffsetJ 
            + 0.5 * (centerPointJ - muscleOffsetJ));
          uint32_t const lowerPointJ = static_cast<uint32_t>(centerPointJ 
            + 0.5 * (muscleOffsetJ + muscleHeight - centerPointJ));
          uint32_t const upperHalfWidth = area / (upperPointJ - muscleOffsetJ);
          uint32_t const upperLeft = centerI - upperHalfWidth;
          uint32_t const upperRight = centerI + upperHalfWidth;
          uint32_t const lowerHalfWidth = area / (lowerPointJ - centerPointJ);
          uint32_t const lowerLeft = centerI - lowerHalfWidth;
          uint32_t const lowerRight = centerI + lowerHalfWidth;

          cv::Mat muscleImg(static_cast<int32_t>(muscleHeight + 2 * muscleOffsetJ),
              static_cast<int32_t>(muscleWidth + 2 * muscleOffsetI),
              CV_8UC3, cv::Scalar(0,0,0));

          // Walls
          cv::line(muscleImg, cv::Point(muscleOffsetI, muscleOffsetJ), 
              cv::Point(muscleOffsetI + muscleWidth, muscleOffsetJ),
              cv::Scalar(255, 255, 255));
          for (uint32_t i = muscleOffsetI; i < rightI; i += tenthWidth) {
            cv::line(muscleImg, cv::Point(i, muscleOffsetJ), 
                cv::Point(i + tenthWidth, muscleOffsetJ - tenthWidth),
                cv::Scalar(255, 255, 255));
          }
          
          cv::line(muscleImg, cv::Point(muscleOffsetI, lowerJ), 
              cv::Point(rightI, lowerJ), cv::Scalar(255, 255, 255));
          for (uint32_t i = muscleOffsetI; i < rightI; i += tenthWidth) {
            cv::line(muscleImg, cv::Point(i, lowerJ + tenthWidth), 
                cv::Point(i + tenthWidth, lowerJ),
                cv::Scalar(255, 255, 255));
          }
          
          // Muscle upper
          cv::line(muscleImg, cv::Point(centerI, muscleOffsetJ), 
              cv::Point(upperLeft, upperPointJ), cv::Scalar(255, 255, 255));

          cv::line(muscleImg, cv::Point(centerI, muscleOffsetJ), 
              cv::Point(upperRight, upperPointJ), cv::Scalar(255, 255, 255));
          
          cv::line(muscleImg, cv::Point(upperLeft, upperPointJ),
              cv::Point(centerI, centerPointJ), cv::Scalar(255, 255, 255));
          
          cv::line(muscleImg, cv::Point(upperRight, upperPointJ),
              cv::Point(centerI, centerPointJ), cv::Scalar(255, 255, 255));
          
          // Muscle lower
          cv::line(muscleImg, cv::Point(centerI, centerPointJ), 
              cv::Point(lowerLeft, lowerPointJ), cv::Scalar(255, 255, 255));
          
          cv::line(muscleImg, cv::Point(centerI, centerPointJ),
              cv::Point(lowerRight, lowerPointJ), cv::Scalar(255, 255, 255));
          
          cv::line(muscleImg,cv::Point(lowerLeft, lowerPointJ), 
              cv::Point(centerI, lowerJ), cv::Scalar(255, 255, 255));

          cv::line(muscleImg, cv::Point(lowerRight, lowerPointJ), 
              cv::Point(centerI, lowerJ), cv::Scalar(255, 255, 255));

          // Forces
          cv::rectangle(muscleImg, cv::Point(muscleOffsetI, centerPointJ), 
              cv::Point(muscleOffsetI + tenthWidth, 
                static_cast<uint32_t>(centerPointJ + force)),
              cv::Scalar(255, 255, 255));


        
          //cv::ellipse(muscleImg, cv::Point(120, 120), cv::Size(10, 10), 0.0,
          //    0.0, 360.0, cv::Scalar(100, 50, 255));
        
          cv::putText(muscleImg, std::to_string(currentPosition), 
              cv::Point(muscleOffsetI + 0.6 * muscleWidth,
                centerPointJ), 0,
              0.5, cv::Scalar(255, 255, 255));
        
          cv::imshow("Two muscle model", muscleImg);

          char key = cv::waitKey(1);

          if (key != -1) {
            if (key == 'q') {
              return false;
            } else if (key == 's') {
              currentLoad += 0.1;
            } else if (key == 'w') {
              currentLoad -= 0.1;
            }
            currentPosition = (currentPosition < 0.0) ? 0.0 
              : (currentPosition > 1.0) ? 1.0 : currentPosition;
          }
        }
        return true;
    }};

    //od4.dataTrigger(opendlv::logic::perception::ObjectType::ID(),
    //    onObjectType);

    od4.timeTrigger(freq, atFrequency);
    
    retCode = 0;
  }
  return retCode;
}
