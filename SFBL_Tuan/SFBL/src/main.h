/*
 * main.h
 *
 *  Created on: Jun 8, 2019
 *      Author: Phuong
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef enum {
 Init = 0,
 StayInBootloader = 1,
 JumpToApplication = 2,
} Bootloader_State;


#endif /* MAIN_H_ */
