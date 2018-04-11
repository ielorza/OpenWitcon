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
 * @file ikSensorDiagnoser.c
 * 
 * @brief Class ikSensorDiagnoser implementation
 */

/* @cond */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ikSensorDiagnoser.h"

void ikSensorDiagnoser_initParams(ikSensorDiagnoserParams *params) {
    
    /* set default parameter values */
    params->nStepsToFault = 1;
    params->tolerance = 1.0;
}

int ikSensorDiagnoser_init(ikSensorDiagnoser *self, const ikSensorDiagnoserParams *params) {
    int i;
    int err = 0;
    
    /* register parameter values */
    if (0.0 < params->nStepsToFault) { 
        self->n = params->nStepsToFault;
    } else {
        for (i = 0; i < 3; i++) self->n = 1;
        if (!err) err = -1;
    }
    if (0 < params->tolerance) {
        self->tol = params->tolerance;
    } else {
        self->tol = 1.0;
        if (!err) err = -2;
    }
    /* initilise fault states */
    for (i = 0; i < 3; i++) self->ok[i] = self->n;

    return err;

}

void ikSensorDiagnoser_step(ikSensorDiagnoser *self, int ok[3], const double signals[3]) {
    int i,j;
    int _ok[3] = {0,0,0};
	/* check tolerances */
	for(i = 0; i < 3; i++) {
		j = i + 1;
		if(j > 2) j = 0;
		if(self->tol > fabs(signals[i] - signals[j])) {
			if (self->ok[i] && self->ok[j]) {
				_ok[i] = 1;
				_ok[j] = 1;
			}
		}
	} 
    /* compute steps left for fault detection */
    for(i = 0; i < 3; i++) {
        self->ok[i]--;
        self->ok[i] = self->ok[i] > 0 ? self->ok[i] : 0;
        if (_ok[i]) self->ok[i] = self->n;
    }
    	
	/*
	####################################################################
	*/	
	if(self->ResetSignal > 0){
		for(i = 0; i < 3; i++) {
		self->ok[i] = 1;
		self->ok[i] = self->n;
		}
	}
    /* set outputs */
    ikSensorDiagnoser_getOutput(self, ok);
}

void ikSensorDiagnoser_getOutput(const ikSensorDiagnoser *self, int ok[3]) {
    int i;

	for(i = 0; i < 3; i++) ok[i] = self->ok[i] > 0;
}

/* @endcond */
