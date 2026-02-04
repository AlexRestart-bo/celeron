#ifndef __AD5668_H__
#define __AD5668_H__

#include <stdint.h>

#define AD5668_WRITE_N             		(0x00000000) /* Write to Input Register N */
#define AD5668_UPDATE_N            		(0x01000000) /* Update DAC Register N */
#define AD5668_WRITE_AND_UPDATE_ALL  	(0x02000000) /* Write to Input Register N, update all */
#define AD5668_WRITE_AND_UPDATE_N    	(0x03000000) /* Write to and update DAC Channel N */
#define AD5668_POWER_DOWN      				(0x04000000) /* Power down/power up DAC */
#define AD5668_CLEAR             			(0x05000000) /* Load clear code register */
#define AD5668_LDAC          					(0x06000000) /* Load !LDAC register*/
#define AD5668_RESET              		(0x07000000) /* Reset*/
#define AD5668_REF              			(0x08000000) /* Set up internal REF register*/

#define AD5668_DAC_A             			(0x00000000) /* DAC A */
#define AD5668_DAC_B             			(0x00100000) /* DAC B */
#define AD5668_DAC_C             			(0x00200000) /* DAC C */
#define AD5668_DAC_D             			(0x00300000) /* DAC D */
#define AD5668_DAC_E             			(0x00400000) /* DAC E */
#define AD5668_DAC_F             			(0x00500000) /* DAC F */
#define AD5668_DAC_G             			(0x00600000) /* DAC G */
#define AD5668_DAC_H             			(0x00700000) /* DAC H */
#define AD5668_DAC_ALL           			(0x00F00000) /* DAC ALL */

#define AD5668_VALUE(value)      			(uint32_t)((value << 4) & 0xFFFF0)

//===============
// POWER DOWN
//===============
#define AD5668_PD_A             			(0x00000001) /* DAC A */
#define AD5668_PD_B             			(0x00000002) /* DAC B */
#define AD5668_PD_C             			(0x00000004) /* DAC C */
#define AD5668_PD_D             			(0x00000008) /* DAC D */
#define AD5668_PD_E             			(0x00000010) /* DAC E */
#define AD5668_PD_F             			(0x00000020) /* DAC F */
#define AD5668_PD_G             			(0x00000040) /* DAC G */
#define AD5668_PD_H             			(0x00000080) /* DAC H */
#define AD5668_PD_ALL             		(0x000000FF) /* DAC ALL */

#define AD5668_PD_PD0									(0x000000100)
#define AD5668_PD_PD1									(0x000000200)
//===============
// Shift Register Contents for Reference Set-Up Command
//===============

#define AD5668_REF_ON									(0x00000001)
#define AD5668_REF_OFF								(0x00000000)
//=========================================================
// USER
//=========================================================
#define DAC_VCO_CRS										AD5668_DAC_A
#define DAC_COIL_CRNT                 AD5668_DAC_B
#define DAC_VCO_FINE                  AD5668_DAC_C
#define DAC_AUX_TEC                  	AD5668_DAC_D
#define DAC_VCSEL_DC                  AD5668_DAC_E
#define DAC_TEC_CTRL               		AD5668_DAC_F
#define DAC_CELL_HTR              		AD5668_DAC_G
#define DAC_RF_ATT                    AD5668_DAC_H


#endif
