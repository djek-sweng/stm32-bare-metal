/*
 * bldr_misc.h
 *
 *  Created on: Jul 31, 2021
 *      Author: djek-sweng
 */

#ifndef BLDR_MISC_H_
#define BLDR_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "bldr_global.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* prototypes */
/*--------------------------------------------------------------------------------------------------------------------*/
int32_t BLDR_InitCommandMessageStruct   (BLDR_CommandMessage_t* message);
int32_t BLDR_InitSystemStateStruct      (BLDR_SystemState_t* systemState);
int32_t BLDR_UpdateCrc16                (BLDR_CommandMessage_t* message);

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* BLDR_MISC_H_ */