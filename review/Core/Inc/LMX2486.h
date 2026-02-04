#ifndef __LMX2486_H__
#define __LMX2486_H__

// It is best to program the N counter last, because doing so initializes the digital lock
// detector and Fastlock circuitry
//=========================================================
// REGISTER ADDRESS MAP
//=========================================================

#define LMX2486_REG_R0    (0x00)
#define LMX2486_REG_R1    (0x03)
#define LMX2486_REG_R2    (0x05)
#define LMX2486_REG_R3    (0x07)
#define LMX2486_REG_R4    (0x09)
#define LMX2486_REG_R5    (0x0B)
#define LMX2486_REG_R6    (0x0D)
#define LMX2486_REG_R7    (0x0F)


//=========================================================
// R0 Latch Map (0x00)
//=========================================================

#define LMX2486_R0_RF_FN(value)       ((value&0x0FFF)<<1)

#define LMX2486_R0_RF_N(value)        ((value&0x07FF)<<13)

#define LMX2486_R0_RF_A(value)				((value&0x03)<<13)
#define LMX2486_R0_RF_B(value)				((value&0x07)<<15)
#define LMX2486_R0_RF_C(value)				((value&0x3F)<<18)

//=========================================================
// R1 Latch Map (0x03)
//=========================================================

#define LMX2486_R1_RF_FD(value)       ((value&0x0FFF)<<4)

#define LMX2486_R1_RF_R(value)        ((value&0x03F)<<16)

#define LMX2486_R1_RF_P               (0x400000)

#define LMX2486_R1_RF_PD              (0x800000)

//=========================================================
// R2 Latch Map (0x05)
//=========================================================

#define LMX2486_R2_IF_N(value)        ((value&0x07FFFFF)<<4)

#define LMX2486_R2_IF_PD              (0x800000)

//=========================================================
// R3 Latch Map (0x07)
//=========================================================

#define LMX2486_R3_IF_R(value)        ((value&0x0FFF)<<4)

// 0  	1X 		95 		uA
// 1  	2X 		190 	uA
// 2  	3X 		285 	uA
// 3  	4X 		380 	uA
// 4  	5X 		475 	uA
// 5  	6X 		570 	uA
// 6  	7X 		665 	uA
// 7  	8X 		760 	uA
// 8  	9X 		855 	uA
// 9  	10X 	950 	uA
// 10  	11X 	1045 	uA
// 11  	12X 	1140 	uA
// 12  	13X 	1235 	uA
// 13  	14X 	1330 	uA
// 14  	15X 	1425 	uA
// 15  	16X 	1520 	uA
#define LMX2486_R3_RF_CPG(value)      ((value&0x0F)<<16)

#define LMX2486_R3_ACCESS_0_SET      (0x100000)
#define LMX2486_R3_ACCESS_1_SET      (0x200000)
#define LMX2486_R3_ACCESS_2_SET      (0x400000)
#define LMX2486_R3_ACCESS_3_SET      (0x800000)  

//=========================================================
// R4 Latch Map (0x09)
//=========================================================
// WARNING
#define LMX2486_R4_R4                   (0x200000)

// Выход MUX
#define LMX2486_R4_MUX_OFF              (0x000000)
#define LMX2486_R4_MUX_LVDD             (0x000010)
#define LMX2486_R4_MUX_LGND             (0x000020)
#define LMX2486_R4_MUX_RF_IF_DLOCK      (0x000030)
#define LMX2486_R4_MUX_RF_DLOCK         (0x000040)
#define LMX2486_R4_MUX_IF_DLOCK         (0x000050)
#define LMX2486_R4_MUX_RF_IF_ALOCK_OD   (0x000060)
#define LMX2486_R4_MUX_RF_ALOCK_OD      (0x000070)
#define LMX2486_R4_MUX_IF_ALOCK_OD      (0x000080)
#define LMX2486_R4_MUX_RF_IF_ALOCK      (0x000090)
#define LMX2486_R4_MUX_RF_ALOCK         (0x0000A0)
#define LMX2486_R4_MUX_IF_ALOCK         (0x0000B0)
#define LMX2486_R4_MUX_IF_R_DIV2        (0x0000C0)
#define LMX2486_R4_MUX_IF_N_DIV2        (0x0000D0)
#define LMX2486_R4_MUX_RF_R_DIV2        (0x0000E0)
#define LMX2486_R4_MUX_RF_N_DIV2        (0x0000F0)
                                           
#define LMX2486_R4_IF_P                 (0x000100)
        
#define LMX2486_R4_RF_CP_NEGATIVE       (0x000000)        
#define LMX2486_R4_RF_CP_POSITIVE       (0x000200)
                                          
#define LMX2486_R4_IF_CPP_NEGATIVE      (0x000000)
#define LMX2486_R4_IF_CPP_POSITIVE      (0x000400)
                                   
#define LMX2486_R4_OSC_OUT_OFF          (0x000000)                                   
#define LMX2486_R4_OSC_OUT_ON           (0x000800)
        
#define LMX2486_R4_OSC_1X               (0x000000)        
#define LMX2486_R4_OSC_2X               (0x001000)
                                           
// Порядок сигма-дельта модулятора         
#define LMX2486_R4_FM_MODE_4X           (0x000000)
#define LMX2486_R4_FM_MODE_3X           (0x00C000)
#define LMX2486_R4_FM_MODE_2X           (0x008000)
#define LMX2486_R4_FM_OFF               (0x004000)
                                           
// Сглаживание                             
#define LMX2486_R4_DITH_OFF             (0x000000)         
#define LMX2486_R4_DITH_WEAK            (0x010000)
#define LMX2486_R4_DITH_STRONG          (0x020000)

#define LMX2486_R4_ATPU                 (0x800000)

//=========================================================
// R5 Latch Map (0x0B)
//=========================================================

#define LMX2486_R5_RF_FN(value)         ((value&0x03FF)<<4)

#define LMX2486_R5_RF_FD(value)         ((value&0x03FF)<<14)

//=========================================================
// R6 Latch Map (0x0D)
//=========================================================

#define LMX2486_R6_RF_TOC(value)        ((value&0x03FFF)<<4)

#define LMX2486_R6_CPF(value)           ((value&0x0F)<<18)

#define LMX2486_R6_CSR_OFF              (0x000000)
#define LMX2486_R6_CSR_DIV2             (0x400000)
#define LMX2486_R6_CSR_DIV4             (0x800000)
#define LMX2486_R6_CSR_DIV16            (0xC00000)

//=========================================================
// R7 Latch Map (0x0F)
//=========================================================
// WARNING
#define LMX2486_R7_R7     (0x0900)

#define LMX2486_R7_RF_CPT (0x0010)

#define LMX2486_R7_IF_CPT (0x0020)

#define LMX2486_R7_RF_RST (0x0040)

#define LMX2486_R7_IF_RST (0x0080)

#define LMX2486_R7_DIV4   (0x2000)



#endif
