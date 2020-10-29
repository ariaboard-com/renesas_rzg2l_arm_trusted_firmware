/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "cpg_regs.h"
#include "cpg.h"
#include <lib/mmio.h>

#define SIZEOF(array)	(sizeof(array)/sizeof(array[0]))

typedef struct {
	uintptr_t reg;
	uint32_t  val;
} CPG_REG_SETTING;

typedef struct {
	CPG_REG_SETTING		clk1_dat;
	CPG_REG_SETTING		clk2_dat;
	CPG_REG_SETTING		stby_dat;
} CPG_PLL_SETDATA_146;

typedef struct {
	CPG_REG_SETTING		clk1_dat;
	CPG_REG_SETTING		clk2_dat;
	CPG_REG_SETTING		clk3_dat;
	CPG_REG_SETTING		clk4_dat;
	CPG_REG_SETTING		clk5_dat;
	CPG_REG_SETTING		stby_dat;
} CPG_PLL_SETDATA_235;

#define	CPG_PLL1_INDEX					(0)
#define	CPG_PLL4_INDEX					(1)
#define	CPG_PLL6_INDEX					(2)

static CPG_PLL_SETDATA_146 cpg_pll_setdata_146[] = {
	{ 
		{ CPG_PLL1_CLK1 , 0x00001901 },
		{ CPG_PLL1_CLK2 , 0x00180601 },
		{ CPG_PLL1_STBY , 0x00000005 }
	},
	{ 
		{ CPG_PLL4_CLK1 , 0x00003203 },
		{ CPG_PLL4_CLK2 , 0x00082400 },
		{ CPG_PLL4_STBY , 0x00010005 }
	},
	{ 
		{ CPG_PLL6_CLK1 , 0x00003E83 },
		{ CPG_PLL6_CLK2 , 0x00082D02 },
		{ CPG_PLL6_STBY , 0x00010001 }
	}
};

#define	CPG_PLL2_INDEX					(0)
#define	CPG_PLL3_INDEX					(1)
#define	CPG_PLL5_INDEX					(2)

static CPG_PLL_SETDATA_235 cpg_pll_setdata_235[] = {
	{
		{ CPG_PLL2_CLK1 , 0x01110111 },
		{ CPG_PLL2_CLK2 , 0x01550100 },
		{ CPG_PLL2_CLK3 , 0x55555506 },
		{ CPG_PLL2_CLK4 , 0x008500FF },
		{ CPG_PLL2_CLK5 , 0x00000016 },
		{ CPG_PLL2_STBY , 0x00000011 }
	},
	{
		{ CPG_PLL3_CLK1 , 0x01110111 },
		{ CPG_PLL3_CLK2 , 0x01550100 },
		{ CPG_PLL3_CLK3 , 0x55555506 },
		{ CPG_PLL3_CLK4 , 0x008500FF },
		{ CPG_PLL3_CLK5 , 0x00000016 },
		{ CPG_PLL3_STBY , 0x00000011 }
	},
	{
		{ CPG_PLL5_CLK1 , 0x01110111 },
		{ CPG_PLL5_CLK2 , 0x01550100 },
		{ CPG_PLL5_CLK3 , 0x00000006 },
		{ CPG_PLL5_CLK4 , 0x007D00FF },
		{ CPG_PLL5_CLK5 , 0x00000016 },
		{ CPG_PLL5_STBY , 0x00010015 }
	}
};

static CPG_REG_SETTING cpg_clk_on_tbl[] = {
	{ (uintptr_t)CPG_CLKON_CM33,            0x00000000 },		/* CM33 */
	{ (uintptr_t)CPG_CLKON_ROM,             0x00010001 },		/* ROM */
	{ (uintptr_t)CPG_CLKON_GIC600,          0x00010001 },		/* GIC600 */
	{ (uintptr_t)CPG_CLKON_IA55,            0x00030003 },		/* IA55 */
	{ (uintptr_t)CPG_CLKON_IM33,            0x00030003 },		/* IM33 */
	{ (uintptr_t)CPG_CLKON_MHU,             0x00000000 },		/* MHU */
	{ (uintptr_t)CPG_CLKON_CST,             0x07ff07ff },		/* CST */
	{ (uintptr_t)CPG_CLKON_SYC,             0x00010001 },		/* SYC */
	{ (uintptr_t)CPG_CLKON_DAMC_REG,        0x00000000 },		/* DMAC */
	{ (uintptr_t)CPG_CLKON_SYSC,            0x00030003 },		/* SYSC */
	{ (uintptr_t)CPG_CLKON_OSTM,            0x00010001 },		/* OSTM */
	{ (uintptr_t)CPG_CLKON_MTU,             0x00000000 },		/* MTU */
	{ (uintptr_t)CPG_CLKON_POE3,            0x00000000 },		/* POE3 */
	{ (uintptr_t)CPG_CLKON_GPT,             0x00000000 },		/* GPT */
	{ (uintptr_t)CPG_CLKON_POEG,            0x00000000 },		/* POEG */
	{ (uintptr_t)CPG_CLKON_WDT,             0x00C300C3 },		/* WDT */
	{ (uintptr_t)CPG_CLKON_DDR,             0x00000000 },		/* DDR */
	{ (uintptr_t)CPG_CLKON_SPI_MULTI,       0x00030003 },		/* SPI_MULTI */
	{ (uintptr_t)CPG_CLKON_SDHI,            0x00ff00ff },		/* SDHI */
	{ (uintptr_t)CPG_CLKON_GPU,             0x00000000 },		/* GPU */
	{ (uintptr_t)CPG_CLKON_ISU,             0x00000000 },		/* Image Scaling Unit */
	{ (uintptr_t)CPG_CLKON_H264,            0x00000000 },		/* H.264 codec */
	{ (uintptr_t)CPG_CLKON_CRU,             0x00000000 },		/* Camera Data Receive Unit */
	{ (uintptr_t)CPG_CLKON_MIPI_DSI,        0x00000000 },		/* MIPI-DSI */
	{ (uintptr_t)CPG_CLKON_LCDC,            0x00000000 },		/* LCDC */
	{ (uintptr_t)CPG_CLKON_SSI,             0x00000000 },		/* Serial Sound Interface */
	{ (uintptr_t)CPG_CLKON_SRC,             0x00000000 },		/* Sampling Rate Converter */
	{ (uintptr_t)CPG_CLKON_USB,             0x00000000 },		/* USB2.0 */
	{ (uintptr_t)CPG_CLKON_ETH,             0x00000000 },		/* ETHER */
	{ (uintptr_t)CPG_CLKON_I2C,             0x00000000 },		/* I2C */
	{ (uintptr_t)CPG_CLKON_SCIF,            0x00010001 },		/* SCIF */
	{ (uintptr_t)CPG_CLKON_SCI,             0x00000000 },		/* SCI */
	{ (uintptr_t)CPG_CLKON_IRDA,            0x00000000 },		/* IrDA */
	{ (uintptr_t)CPG_CLKON_RSPI,            0x00000000 },		/* SPI */
	{ (uintptr_t)CPG_CLKON_CANFD,           0x00000000 },		/* CAN */
	{ (uintptr_t)CPG_CLKON_GPIO,            0x00010001 },		/* GPIO */
	{ (uintptr_t)CPG_CLKON_TSIPG,           0x00000000 },		/* TSIPG */
	{ (uintptr_t)CPG_CLKON_JAUTH,           0x00010001 },		/* JAUTH */
	{ (uintptr_t)CPG_CLKON_OTP,             0x00030003 },		/* OTP */
	{ (uintptr_t)CPG_CLKON_ADC,             0x00000000 },		/* ADC */
	{ (uintptr_t)CPG_CLKON_TSU,             0x00000000 },		/* Thermal Sensor Unit */
	{ (uintptr_t)CPG_CLKON_BBGU,            0x00010001 },		/* BBGU */
	{ (uintptr_t)CPG_CLKON_AXI_ACPU_BUS,    0x000f000f },		/* AXI_ACPU_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_MCPU_BUS,    0x07ff07ff },		/* AXI_MCPU_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_COM_BUS,     0x00030003 },		/* AXI_COM_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_VIDEO_BUS,   0x00030003 },		/* AXI_VIDEO_BUS */
	{ (uintptr_t)CPG_CLKON_PERI_COM,        0x00030003 },		/* PERI_COM */
	{ (uintptr_t)CPG_CLKON_REG0_BUS,        0x000f000f },		/* REG0_BUS */
	{ (uintptr_t)CPG_CLKON_REG1_BUS,        0x00030003 },		/* REG1_BUS */
	{ (uintptr_t)CPG_CLKON_PERI_CPU,        0x000f000f },		/* PERI_CPU */
	{ (uintptr_t)CPG_CLKON_PERI_VIDEO,      0x00070007 },		/* PERI_VIDEO */
	{ (uintptr_t)CPG_CLKON_PERI_DDR,        0x00010001 },		/* PERI_DDR */
	{ (uintptr_t)CPG_CLKON_AXI_TZCDDR,      0x001f001f },		/* AXI_TZCDDR */
	{ (uintptr_t)CPG_CLKON_MTGPGS,          0x00030003 },		/* MTGPGS */
	{ (uintptr_t)CPG_CLKON_AXI_DEFAULT_SLV, 0x00010001 },		/* AXI_DEFAULT_SLV */
};

static CPG_REG_SETTING cpg_reset_tbl[] = {
	{ (uintptr_t)CPG_RST_CM33,              0x00000000 },		/* CM33 */
	{ (uintptr_t)CPG_RST_ROM,               0x00010001 },		/* ROM */
	{ (uintptr_t)CPG_RST_GIC600,            0x00030003 },		/* GIC600 */
	{ (uintptr_t)CPG_RST_IA55,              0x00010001 },		/* IA55 */
	{ (uintptr_t)CPG_RST_IM33,              0x00010001 },		/* IM33 */
	{ (uintptr_t)CPG_RST_MHU,               0x00000000 },		/* MHU */
	{ (uintptr_t)CPG_RST_SYC,               0x00010001 },		/* SYC */
	{ (uintptr_t)CPG_RST_DMAC,              0x00010001 },		/* DMAC */
	{ (uintptr_t)CPG_RST_SYSC,              0x00070007 },		/* SYSC */
	{ (uintptr_t)CPG_RST_OSTM,              0x00010001 },		/* OSTM */
	{ (uintptr_t)CPG_RST_MTU,               0x00000000 },		/* MTU */
	{ (uintptr_t)CPG_RST_POE3,              0x00000000 },		/* POE3 */
	{ (uintptr_t)CPG_RST_GPT,               0x00000000 },		/* GPT */
	{ (uintptr_t)CPG_RST_POEG,              0x00000000 },		/* POEG */
	{ (uintptr_t)CPG_RST_WDT,               0x00090009 },		/* WDT */
	{ (uintptr_t)CPG_RST_DDR,               0x00000000 },		/* DDR */
	{ (uintptr_t)CPG_RST_SPI,               0x00010001 },		/* SPI_MULTI */
	{ (uintptr_t)CPG_RST_SDHI,              0x00030003 },		/* SDHI */
	{ (uintptr_t)CPG_RST_GPU,               0x00000000 },		/* GPU */
	{ (uintptr_t)CPG_RST_ISU,               0x00000000 },		/* Image Scaling Unit */
	{ (uintptr_t)CPG_RST_H264,              0x00000000 },		/* H.264 codec */
	{ (uintptr_t)CPG_RST_CRU,               0x00000000 },		/* Camera Data Receive Unit */
	{ (uintptr_t)CPG_RST_MIPI_DSI,          0x00000000 },		/* MIPI-DSI */
	{ (uintptr_t)CPG_RST_LCDC,              0x00000000 },		/* LCDC */
	{ (uintptr_t)CPG_RST_SSIF,              0x00000000 },		/* Serial Sound Interface */
	{ (uintptr_t)CPG_RST_SRC,               0x00000000 },		/* Sampling Rate Converter */
	{ (uintptr_t)CPG_RST_USB,               0x00000000 },		/* USB2.0 */
	{ (uintptr_t)CPG_RST_ETH,               0x00000000 },		/* ETHER */
	{ (uintptr_t)CPG_RST_I2C,               0x00000000 },		/* I2C */
	{ (uintptr_t)CPG_RST_SCIF,              0x00010001 },		/* SCIF */
	{ (uintptr_t)CPG_RST_SCI,               0x00000000 },		/* SCI */
	{ (uintptr_t)CPG_RST_IRDA,              0x00000000 },		/* IrDA */
	{ (uintptr_t)CPG_RST_RSPI,              0x00000000 },		/* SPI */
	{ (uintptr_t)CPG_RST_CANFD,             0x00000000 },		/* CAN */
	{ (uintptr_t)CPG_RST_GPIO,              0x00070007 },		/* GPIO */
	{ (uintptr_t)CPG_RST_TSIPG,             0x00000000 },		/* TSIPG */
	{ (uintptr_t)CPG_RST_JAUTH,             0x00010001 },		/* JAUTH */
	{ (uintptr_t)CPG_RST_OTP,               0x00010001 },		/* OTP */
	{ (uintptr_t)CPG_RST_ADC,               0x00000000 },		/* ADC */
	{ (uintptr_t)CPG_RST_TSU,               0x00000000 },		/* Thermal Sensor Unit */
	{ (uintptr_t)CPG_RST_BBGU,              0x00000000 },		/* BBGU */
	{ (uintptr_t)CPG_RST_AXI_ACPU_BUS,      0x00010001 },		/* AXI_ACPU_BUS */
	{ (uintptr_t)CPG_RST_AXI_MCPU_BUS,      0x00010001 },		/* AXI_MCPU_BUS */
	{ (uintptr_t)CPG_RST_AXI_COM_BUS,       0x00010001 },		/* AXI_COM_BUS */
	{ (uintptr_t)CPG_RST_AXI_VIDEO_BUS,     0x00010001 },		/* AXI_VIDEO_BUS */
	{ (uintptr_t)CPG_RST_PERI_COM,          0x00010001 },		/* PERI_COM */
	{ (uintptr_t)CPG_RST_REG1_BUS,          0x00010001 },		/* REG1_BUS */
	{ (uintptr_t)CPG_RST_REG0_BUS,          0x00010001 },		/* REG0_BUS */
	{ (uintptr_t)CPG_RST_PERI_CPU,          0x00010001 },		/* PERI_CPU */
	{ (uintptr_t)CPG_RST_PERI_VIDEO,        0x00010001 },		/* PERI_VIDEO */
	{ (uintptr_t)CPG_RST_PERI_DDR,          0x00010001 },		/* PERI_DDR */
	{ (uintptr_t)CPG_RST_AXI_TZCDDR,        0x001f001f },		/* AXI_TZCDDR */
	{ (uintptr_t)CPG_RST_MTGPGS,            0x00030003 },		/* MTGPGS */
	{ (uintptr_t)CPG_RST_AXI_DEFAULT_SLV,   0x00010001 },		/* AXI_DEFAULT_SLV */
};

static CPG_REG_SETTING cpg_select_tbl[] = {
	{ (uintptr_t)CPG_PL1_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2_DDIV,              0x11110000 },
	{ (uintptr_t)CPG_PL3A_DDIV,             0x01110300 },
	{ (uintptr_t)CPG_PL3B_DDIV,             0x00010000 },
	{ (uintptr_t)CPG_PL5_SDIV,              0x01010000 },
	{ (uintptr_t)CPG_PL6_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2SDHI_DSEL,          0x00110022 },
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 },
	{ (uintptr_t)CPG_PL3_SSEL,              0x01000000 },
	{ (uintptr_t)CPG_PL6_SSEL,              0x10000000 },
	{ (uintptr_t)CPG_PL6_ETH_SSEL,          0x00010000 },
	{ (uintptr_t)CPG_OTHERFUNC1_REG,        0x00010000 }
};

#if 0
/* 
 * for dynamic switching clock DIVIDERs and SELECTORs
 * Only the following must be set.
 *
 *    - CPG_PL1_DDIV
 *    - CPG_PL2_DDIV
 *    - CPG_PL3A_DDIV
 *    - CPG_PL3B_DDIV
 *    - CPG_PL6_DDIV
 *    - CPG_PL2SDHI_DSEL
 *    - CPG_PL4_DSEL
 */ 
static CPG_REG_SETTING cpg_dynamic_sel_tbl[] = {
	{ (uintptr_t)CPG_PL1_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2_DDIV,              0x11110000 },
	{ (uintptr_t)CPG_PL3A_DDIV,             0x01110300 },
	{ (uintptr_t)CPG_PL3B_DDIV,             0x00010000 },
	{ (uintptr_t)CPG_PL6_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2SDHI_DSEL,          0x00110022 },
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 },
};

/* 
 * for static switching clock DIVIDERs and SELECTORs
 * Only the following must be set.
 *
 *    - CPG_PL5_SDIV
 *    - CPG_PL3_SSEL
 *    - CPG_PL6_SSEL
 *    - CPG_PL6_ETH_SSEL
 *    - CPG_OTHERFUNC1_REG
 */
static CPG_REG_SETTING cpg_static_sel_tbl[] = {
	{ (uintptr_t)CPG_PL5_SDIV,              0x01010000 },
	{ (uintptr_t)CPG_PL3_SSEL,              0x01000000 },
	{ (uintptr_t)CPG_PL6_SSEL,              0x10000000 },
	{ (uintptr_t)CPG_PL6_ETH_SSEL,          0x00010000 },
	{ (uintptr_t)CPG_OTHERFUNC1_REG,        0x00010000 }
};
#endif

#if 0
static void cpg_pll_stop(uintptr_t reg)
{
	uint32_t set = 0x00010000;
	
	mmio_write_32(reg, set);
}
#endif

static void cpg_pll_start_146(uint32_t index)
{
	mmio_write_32(cpg_pll_setdata_146[index].clk1_dat.reg, cpg_pll_setdata_146[index].clk1_dat.val);
	mmio_write_32(cpg_pll_setdata_146[index].clk2_dat.reg, cpg_pll_setdata_146[index].clk2_dat.val);
	mmio_write_32(cpg_pll_setdata_146[index].stby_dat.reg, cpg_pll_setdata_146[index].stby_dat.val);
}

static void cpg_pll_start_235(uint32_t index)
{
	mmio_write_32(cpg_pll_setdata_235[index].clk1_dat.reg, cpg_pll_setdata_235[index].clk1_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk2_dat.reg, cpg_pll_setdata_235[index].clk2_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk3_dat.reg, cpg_pll_setdata_235[index].clk3_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk4_dat.reg, cpg_pll_setdata_235[index].clk4_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk5_dat.reg, cpg_pll_setdata_235[index].clk5_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].stby_dat.reg, cpg_pll_setdata_235[index].stby_dat.val);
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
	uint32_t val = 0;
	
	/* PLL4 startup */
	/* PLL4 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);
	
	/* Set PLL4 to normal mode */
	/* mmio_write_32(CPG_PLL4_STBY, (PLL4_STBY_RESETB_WEN | PLL4_STBY_RESETB)); */
	cpg_pll_start_146(CPG_PLL4_INDEX);
	
	/* PLL4 normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);
	
	/* PLL5 startup */
	/* PLL5 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) != 0);
	
#if 0
	/* Divided ratio setting of PLL5 Divide 3000MHz by 32 and set it within 5.08-148.5MHz. */
	/* val = PL5_SDIV_DIVSDIB_WEN | PL5_SDIV_DIVDSA_WEN | PL5_SDIV_DIVDSIA_SET_1_4 | PL5_SDIV_DIVDSIB_SET_1_8; */
	val = PL5_SDIV_DIVSDIB_WEN | PL5_SDIV_DIVDSA_WEN | PL5_SDIV_DIVDSIA_SET_1_1 | PL5_SDIV_DIVDSIB_SET_1_1;
	mmio_write_32(CPG_PL5_SDIV, val);
#endif
	
	/* PLL5 set to normal mode */
	/* val = mmio_read_32(CPG_PLL5_STBY) | PLL5_STBY_RESETB_WEN | PLL5_STBY_RESETB; */
	/* mmio_write_32(CPG_PLL5_STBY, val); */
	cpg_pll_start_235(CPG_PLL5_INDEX);
	
	/* PLL5 Normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) == 0);
	
	/* PLL6 startup */
	/* PLL6 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) != 0);
	
	/* Set PLL6 to normal mode */
	/* val = mmio_read_32(CPG_PLL6_STBY) | PLL6_STBY_RESETB_WEN | PLL6_STBY_RESETB; */
	/* mmio_write_32(CPG_PLL6_STBY, val); */
	cpg_pll_start_146(CPG_PLL6_INDEX);
	
	/* PLL6 Normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) == 0);
	
}

static void cpg_div_sel_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < SIZEOF(cpg_select_tbl); cnt++) {
		mmio_write_32(cpg_select_tbl[cnt].reg, cpg_select_tbl[cnt].val);
	}
	
	/* Wait for completion of settings */
	while(mmio_read_32(CPG_CLKSTATUS) != 0);
}

static void cpg_clk_on_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < SIZEOF(cpg_clk_on_tbl); cnt++) {
		mmio_write_32(cpg_clk_on_tbl[cnt].reg, cpg_clk_on_tbl[cnt].val);
	}
		
	/* FIXME */
	/* Wait for completion of settings */
	while((mmio_read_32(CPG_CLKMON_AXI_DEFAULT_SLV) & BIT0_ON) == 0 );
}

static void cpg_reset_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < SIZEOF(cpg_reset_tbl); cnt++) {
		mmio_write_32(cpg_reset_tbl[cnt].reg, cpg_reset_tbl[cnt].val);
	}
		
	/* FIXME */
	/* Wait for completion of settings */
	while((mmio_read_32(CPG_RSTMON_AXI_DEFAULT_SLV) & BIT0_ON) == 0 );
}

void cpg_setup(void)
{
	cpg_div_sel_setup();
	cpg_pll_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
}