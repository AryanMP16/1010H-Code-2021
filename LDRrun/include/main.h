/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES
//Variables
/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS
//includes
#include "api.h"
#include "drive.h"
#include "run.h"
#include "display.h"

static std::string RUNFILE= "/usd/1010H.txt";
static std::string SAVEFOLDER= "Savefile";
//namespaces
using namespace pros;
using namespace std;
//rerun stuff
static string RUNF = "RUNF.txt";
// using namespace pros::literals;
// using namespace okapi;
//externs
/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */

//Controller
 extern Controller master;
 extern Controller partner;

 //Objects
 extern dpidClass chassis;
 extern opClass base;

 //globals
 extern int clawTargetR;
 extern int clawTargetL;
 extern bool goingToCrash;
 extern int batmeter;
 extern string readfile;
 extern char readfilechar;
 extern char hfile [];

//Task
 extern Task acc_task;
//Motors
 extern Motor driveRB;
 extern Motor driveRF;
 extern Motor driveLB;
 extern Motor driveLF;
 extern Motor lClaw;
 extern Motor rClaw;
 extern Motor roller;
 extern Motor futureUse4;

 extern ADILineSensor outer_limitL;
    extern ADILineSensor outer_limitR;
    extern Distance backR;
    extern Distance backL;
 ///////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif
