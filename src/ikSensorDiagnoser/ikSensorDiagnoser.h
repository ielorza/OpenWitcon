/*
Copyright (C) 2017 IK4-IKERLAN

This file is part of OpenWitcon.
 
OpenWitcon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
OpenWitcon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with OpenWitcon. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file ikSensorDiagnoser.h
 * 
 * @brief Class ikSensorDiagnoser interface
 */

#ifndef IKSENSORDIAGNOSER_H
#define IKSENSORDIAGNOSER_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @struct ikSensorDiagnoser
     * @brief Sensor diagnoser based on 3-way comparison
     * 
     * Instances of this type are sensor diagnosers based on 3-way comparison.
     * They take 3 different sensor signals for a single physical quantity and
     * decide, based on their differences over time, which are sound.
     * 
     * @par Inputs
     * @li signals: sensor signals, specify via @link ikSensorDiagnoser_step @endlink
     * 
     * @par Outputs
     * @li diagnosis: 1 for sound sensors, 0 for faulty ones, get via @link ikSensorDiagnoser_step @endlink or @link ikSensorDiagnoser_getOutput @endlink
     * 
     * @par Methods
     * @li @link ikSensorDiagnoser_initParams @endlink initialise initialisation parameter structure
     * @li @link ikSensorDiagnoser_init @endlink initialise an instance
     * @li @link ikSensorDiagnoser_step @endlink execute preriodic calculations
     * @li @link ikSensorDiagnoser_getOutput @endlink get last diagnosis
     */
    typedef struct ikSensorDiagnoser {
        /* @cond */
        int ok[3];
        int n;
        double tol;
		int nSteps;/* ################################################*/
        /* @endcond */
    } ikSensorDiagnoser;
    
    /**
     * @struct ikSensorDiagnoserParams
     * @brief Sensor diagnoser initialisation parameters
     */
    typedef struct ikSensorDiagnoserParams {
        int nStepsToFault; /**<number of steps the difference between two signals must exceed tolerance to trigger a fault detection*/
        double tolerance; /**<tolerance*/
		int nIgnoredSteps; /**<number of steps ignored to avoid initialization errors *//* ################################################*/
    } ikSensorDiagnoserParams;
    
    /**
     * Initialise initialisation parameter structure
     * @param params initialisation parameter structure
     */
    void ikSensorDiagnoser_initParams(ikSensorDiagnoserParams *params);
    
    /**
     * Initialise instance
     * @param self instance
     * @param params initialisation parameters
     * @return error code:
     * @li 0: no error
     * @li -1: invalid number of steps to fault, must be positive
     * @li -2: invalid tolerance, must be positive
     */
    int ikSensorDiagnoser_init(ikSensorDiagnoser *self, const ikSensorDiagnoserParams *params);
    
    /**
     * Execute periodic calculations
     * @param self instance
     * @param ok output array: 1 for sound sensors, 0 for faulty ones
     * @param signals sensor signals
     */
    void ikSensorDiagnoser_step(ikSensorDiagnoser *self, int ok[3], const double signals[3]);
    
    /**
     * Get last diagnosis
     * @param self instance
     * @param ok output array: 1 for sound sensors, 0 for faulty ones
     */
    void ikSensorDiagnoser_getOutput(const ikSensorDiagnoser *self, int ok[3]);


#ifdef __cplusplus
}
#endif

#endif /* IKSENSORDIAGNOSER_H */

