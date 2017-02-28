// license:GPL-2.0+
// copyright-holders:Felipe Sanches
/***************************************************************************
  This driver covers only the Operator Panel of the BancTec 91690 Document Processor equipment

  Author: Felipe Sanches <juca@members.fsf.org>

  Maintainence Manual: <https://garoa.net.br/w/images/PAINEL_BANCTEC_91690.PDF>

  The display is considered a replaceable part, not repairable. Therefore the manual
  has no circuit description or schematic. And so, much of the below is guesswork.

  Machine starts up and clears video ram, then nothing more. Location of videoram
  in the memory map is unknown, left at 8000 for now.

*/

#include "emu.h"
#include "cpu/mcs51/mcs51.h"
#include "cpu/m6800/m6800.h"
#include "video/mc6845.h"
#include "screen.h"

class banctec_state : public driver_device
{
public:
	banctec_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_palette(*this, "palette")
		, m_video_address(0)
		, m_maincpu(*this, "maincpu")
		, m_videoram(*this, "videoram")
		, m_p_chargen(*this, "chargen")
	{ }

	MC6845_UPDATE_ROW(crtc_update_row);
	MC6845_ON_UPDATE_ADDR_CHANGED(crtc_addr);
	DECLARE_WRITE8_MEMBER(videoram_w);
	required_device<palette_device> m_palette;

private:
	u8 m_video_address;
	virtual void machine_reset() override;
	required_device<cpu_device> m_maincpu;
	required_shared_ptr<u8> m_videoram;
	required_region_ptr<u8> m_p_chargen;
};

static ADDRESS_MAP_START( banctec_mem , AS_PROGRAM, 8, banctec_state )
	AM_RANGE(0x0000, 0x07ff) AM_ROM
	AM_RANGE(0x0800, 0xffff) AM_RAM /* Probably wrong. Must be verified on pcb! */
ADDRESS_MAP_END

static ADDRESS_MAP_START( banctec_mcu_mem , AS_PROGRAM, 8, banctec_state )
	AM_RANGE(0x0000, 0x00ff) AM_RAM /* Probably wrong. Must be verified on pcb! */
	AM_RANGE(0x2000, 0x2000) AM_DEVREADWRITE("crtc", mc6845_device, status_r, address_w)
	AM_RANGE(0x2001, 0x2001) AM_DEVREADWRITE("crtc", mc6845_device, register_r, register_w)
	AM_RANGE(0x2003, 0x2003) AM_WRITE(videoram_w)
	AM_RANGE(0x8000, 0x80ff) AM_RAM AM_SHARE("videoram") /* Probably wrong. Must be verified on pcb! */
	AM_RANGE(0xe000, 0xffff) AM_ROM AM_REGION("mcu", 0x0000)
ADDRESS_MAP_END

void banctec_state::machine_reset()
{
}


/****************************
* Video/Character functions *
****************************/

WRITE8_MEMBER( banctec_state::videoram_w )
{
	m_videoram[m_video_address] = data;
	m_video_address++;
}

/* ROCKWELL 6545 - Transparent Memory Addressing */

MC6845_UPDATE_ROW( banctec_state::crtc_update_row )
{
	const rgb_t *palette = m_palette->palette()->entry_list_raw();
	u8 chr,gfx;
	u16 mem,x;
	u32 *p = &bitmap.pix32(y);

	for (x = 0; x < x_count; x++)
	{
		mem = (ma + x) & 0xff;
		chr = m_videoram[mem];
		gfx = m_p_chargen[chr | (ra << 8)] ^ ((x == cursor_x) ? 0xff : 0);

		/* Display a scanline of a character (8 pixels) */
		*p++ = palette[BIT(gfx, 7)];
		*p++ = palette[BIT(gfx, 6)];
		*p++ = palette[BIT(gfx, 5)];
		*p++ = palette[BIT(gfx, 4)];
		*p++ = palette[BIT(gfx, 3)];
		*p++ = palette[BIT(gfx, 2)];
		*p++ = palette[BIT(gfx, 1)];
		*p++ = palette[BIT(gfx, 0)];
	}
}

MC6845_ON_UPDATE_ADDR_CHANGED(banctec_state::crtc_addr)
{
	/* What is this function meant to do ? */
	m_video_address = address;
}

/******************************
* Graphics Decode Information *
******************************/

const gfx_layout banctec_gfx_layout =
{
	8, 8,               /* 8x8 characters */
	256,                /* 256 characters */
	1,                  /* 1 bits per pixel */
	{0},                /* no bitplanes; 1 bit per pixel */
	{0, 1, 2, 3, 4, 5, 6, 7},
	{0 * 256*8, 1 * 256*8, 2 * 256*8, 3 * 256*8, 4 * 256*8, 5 * 256*8, 6 * 256*8, 7 * 256*8},
	8                 /* size of one char */
};

static GFXDECODE_START( banctec )
	GFXDECODE_ENTRY( "chargen", 0x00000, banctec_gfx_layout, 0, 1 )
GFXDECODE_END

static MACHINE_CONFIG_START( banctec, banctec_state )
	/* basic machine hardware */

	MCFG_CPU_ADD("maincpu", I80C31, XTAL_11_0592MHz)
	MCFG_CPU_PROGRAM_MAP(banctec_mem)

	MCFG_CPU_ADD("mcu", M6803, 4000000)     /* Actual MCU is a Motorola 6803 and the clock frequency is still unknown */
	MCFG_CPU_PROGRAM_MAP(banctec_mcu_mem)

// The video signal is generated by a R6545EAP character generator chip
// The U20 EPROM holds the image data for the character set.

	// video hardware
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_SIZE((52+1)*8, (31+1)*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 40*8-1, 0*8, 25*8-1)
	MCFG_SCREEN_UPDATE_DEVICE("crtc", mc6845_device, screen_update)
	MCFG_PALETTE_ADD_MONOCHROME("palette")
	MCFG_GFXDECODE_ADD("gfxdecode", "palette", banctec)

	MCFG_MC6845_ADD("crtc", R6545_1, "screen", XTAL_2MHz) /* (?) */
	MCFG_MC6845_SHOW_BORDER_AREA(false)
	MCFG_MC6845_CHAR_WIDTH(8)
	MCFG_MC6845_UPDATE_ROW_CB(banctec_state, crtc_update_row)
	MCFG_MC6845_ADDR_CHANGED_CB(banctec_state, crtc_addr)

MACHINE_CONFIG_END

ROM_START(banctec)
	ROM_REGION(0x800, "maincpu", 0)
	ROM_LOAD("banctec_eseries_panel_opnl.u20", 0x000, 0x800, CRC(c2ab9c06) SHA1(a296589034f656790ad5ffbce028dd846a40cf03))

	ROM_REGION(0x2000, "mcu", 0)
	ROM_LOAD("banctec_eseries_panel.u8", 0x0000, 0x2000, CRC(f3335e0a) SHA1(5ca45fdcb7ef45a65c28c79abfa9ebb7a8a06619))

	ROM_REGION(0x1000, "chargen", 0)
	ROM_LOAD("banctec_eseries_panel.u20", 0x0000, 0x1000, CRC(5b6ecec9) SHA1(35aff8f965bce77205e3a43d71e39097585091a7))
ROM_END

/***************************************************************************

  Game driver(s)

***************************************************************************/

/*    YEAR  NAME      PARENT    COMPAT  MACHINE   INPUT     INIT    MONITOR COMPANY   FULLNAME */
CONS( 1989, banctec, 0,        0,      banctec, 0, driver_device, 0,       "DALE Electronics",  "BancTec ESeries Panel", MACHINE_NOT_WORKING | MACHINE_NO_SOUND)
