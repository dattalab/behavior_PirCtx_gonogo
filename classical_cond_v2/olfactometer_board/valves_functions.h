/*
 * valves_functions.h
 *
 *  Created on: May 13, 2015
 *      Author: ludoviccacheux
 */

#ifndef VALVES_FUNCTIONS_H_
#define VALVES_FUNCTIONS_H_

void OpenValve(int valveNum);

void CloseValve(int valveNum);

void olfStim(int valveNum);

void idleOlfacto();

void measureFlow();

void displayFlowRates();

void changeMFCFlow(int type, float flow);


#endif /* VALVES_FUNCTIONS_H_ */
