#ifndef __AD7794DEF_H__
#define __AD7794DEF_H__

//=========================================================
// REGISTER ADDRESS MAP
//=========================================================

#define AD7794_REG_wCOMM 				((0x00)<<3)  // 8 - bit
#define AD7794_REG_rSTAT 				((0x00)<<3)  // 8 - bit
#define AD7794_REG_MR 					((0x01)<<3)  // 16 - bit
#define AD7794_REG_CFG					((0x02)<<3)  // 16 - bit
#define AD7794_REG_DATA 				((0x03)<<3)  // 24 - bit
#define AD7794_REG_ID 					((0x04)<<3)  // 8 - bit
#define AD7794_REG_IO 					((0x05)<<3)  // 8 - bit
#define AD7794_REG_OFFSET 			((0x06)<<3)  // 24 - bit
#define AD7794_REG_FS 					((0x07)<<3)	 // 24 - bit

//=========================================================
// Communications register (0x00)
//=========================================================

#define AD7794_CR_nWEN						0x80

#define AD7794_CR_R_nW						0x40

#define AD7794_CR_REG_wCOMM_rSTAT		0x00
#define AD7794_CR_REG_MODE					0x08
#define AD7794_CR_REG_CFG						0x10
#define AD7794_CR_REG_DAT						0x18
#define AD7794_CR_REG_ID						0x20
#define AD7794_CR_REG_IO						0x28
#define AD7794_CR_REG_OFFSET				0x30
#define AD7794_CR_REG_FULL_SCALE		0x38

#define AD7794_CR_CREAD						0x04

//=========================================================
// STATUS  (0x00)
//=========================================================

#define AD7794_STAT_nRDY					0x80
#define AD7794_STAT_ERR						0x40
#define AD7794_STAT_NOXREF				0x20

//=========================================================
// Mode register (0x01)
//=========================================================

#define AD7794_MR_MODE_MASK								0xE000
#define AD7794_MR_MODE_CONT_CONV					0x0000
#define AD7794_MR_MODE_SINGLE_CONV				0x2000
#define AD7794_MR_MODE_IDLE								0x4000
#define AD7794_MR_MODE_POWER_DOWN					0x6000
#define AD7794_MR_MODE_INT_ZERO_SCALE_CAL	0x8000
#define AD7794_MR_MODE_INT_FULL_SCALE_CAL	0xA000
#define AD7794_MR_MODE_SYS_ZERO_SCALE_CAL	0xC000
#define AD7794_MR_MODE_SYS_FULL_SCALE_CAL	0xE000

#define AD7794_MR_MODE_AMP_CM				0x0200
#define AD7794_MR_MODE_CHOP_DIS			0x0010

#define AD7794_MR_CLK_INT_64K_PIN_DISABLED	0x0000
#define AD7794_MR_CLK_INT_64K_PIN_ENABLED		0x0040
#define AD7794_MR_CLK_EXT_64K								0x0080
#define AD7794_MR_CLK_EXT_DIVIDE_BY_2				0x00C0

#define AD7794_MR_UPDATE_RATE_470Hz				1
#define AD7794_MR_UPDATE_RATE_242Hz				2
#define AD7794_MR_UPDATE_RATE_123Hz				3
#define AD7794_MR_UPDATE_RATE_62Hz				4
#define AD7794_MR_UPDATE_RATE_50Hz				5
#define AD7794_MR_UPDATE_RATE_39Hz				6
#define AD7794_MR_UPDATE_RATE_33Hz2				7
#define AD7794_MR_UPDATE_RATE_19Hz6				8
#define AD7794_MR_UPDATE_RATE_16Hz7_50Hz	9
#define AD7794_MR_UPDATE_RATE_16Hz7				10
#define AD7794_MR_UPDATE_RATE_12Hz5				11
#define AD7794_MR_UPDATE_RATE_10Hz				12
#define AD7794_MR_UPDATE_RATE_8Hz33				13
#define AD7794_MR_UPDATE_RATE_6Hz25				14
#define AD7794_MR_UPDATE_RATE_4Hz17				15

//=========================================================
// Configuration register (0x02)
//=========================================================

#define AD7794_CFG_VBIAS_DISABLED			0x0000
#define AD7794_CFG_VBIAS_AIN1					0x4000
#define AD7794_CFG_VBIAS_AIN2					0x8000
#define AD7794_CFG_VBIAS_AIN3					0xC000

#define AD7794_CFG_BURNOUT_CURRENT		0x2000
#define AD7794_CFG_UNI_nBIPOLAR				0x1000
#define AD7794_CFG_BOOST							0x0800

#define AD7794_CFG_GAIN1							0x0000
#define AD7794_CFG_GAIN2							0x0100
#define AD7794_CFG_GAIN4							0x0200
#define AD7794_CFG_GAIN8							0x0300
#define AD7794_CFG_GAIN16							0x0400
#define AD7794_CFG_GAIN32							0x0500
#define AD7794_CFG_GAIN64							0x0600
#define AD7794_CFG_GAIN128						0x0700

#define AD7794_CFG_REFSEL_REFIN1			0x0000
#define AD7794_CFG_REFSEL_REFIN2			0x0040
#define AD7794_CFG_REFSEL_INT1V17			0x0080

#define AD7794_CFG_REF_DET						0x0020
#define AD7794_CFG_REF_BUF						0x0010

#define AD7794_CFG_CH_MASK				  	15
#define AD7794_CFG_CH1								0
#define AD7794_CFG_CH2								1							
#define AD7794_CFG_CH3								2
#define AD7794_CFG_CH4								3
#define AD7794_CFG_CH5								4
#define AD7794_CFG_CH6								5
#define AD7794_CFG_CH_TEMP						6
#define AD7794_CFG_CH_AVDD						7

//=========================================================
// IO Register (0x05)
//=========================================================

#define AD7794_IO_IOEN					0x40

#define AD7794_IO_IO1DAT				0x10
#define AD7794_IO_IO2DAT				0x20

#define AD7794_IO_IEXCEN_DISABLED		0x00
#define AD7794_IO_IEXCEN_10uA				0x01
#define AD7794_IO_IEXCEN_210uA			0x02
#define AD7794_IO_IEXCEN_1mA				0x03

#define AD7794_IO_IEXCDIR0					0x04
#define AD7794_IO_IEXCDIR1					0x08





#endif
