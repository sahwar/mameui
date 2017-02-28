// license:BSD-3-Clause
// copyright-holders:Barry Rodewald

#include "emu.h"
#include "machine/octo_kbd.h"
#include "machine/keyboard.ipp"


octopus_keyboard_device::octopus_keyboard_device(const machine_config& mconfig, const char* tag, device_t* owner, uint32_t clock)
	: buffered_rs232_device(mconfig, OCTOPUS_KEYBOARD, "Octopus Keyboard", tag, owner, 0, "octopus_kb", __FILE__)
	, device_matrix_keyboard_interface(mconfig, *this, "LINE0", "LINE1", "LINE2", "LINE3", "LINE4", "LINE5", "LINE6", "LINE7", "LINE8", "LINE9", "LINEA", "LINEB", "LINEC", "LINED", "LINEE", "LINEF")
{
}


void octopus_keyboard_device::received_byte(uint8_t data)
{
	// TODO: figure out received data format (used for Shift Lock and Caps Lock LEDs)
	printf("KBD: Received byte %02x\n",data);
}

void octopus_keyboard_device::key_make(uint8_t row, uint8_t column)
{
	if (row != 0x0eU)
		typematic_start(row, column, attotime::from_msec(m_delay), attotime::from_msec(m_repeat));
	else
		typematic_restart(attotime::from_msec(m_delay), attotime::from_msec(m_repeat));

	transmit_byte((row << 3) | column);
}

void octopus_keyboard_device::key_repeat(uint8_t row, uint8_t column)
{
	transmit_byte((row << 3) | column);
}

void octopus_keyboard_device::key_break(uint8_t row, uint8_t column)
{
	device_matrix_keyboard_interface::key_break(row, column);
	transmit_byte(0x80U | (row << 3) | column);
}

static INPUT_PORTS_START( octopus_keyboard )

	// 0x00
	PORT_START("LINE0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("J") PORT_CODE(KEYCODE_J) PORT_CHAR('j') PORT_CHAR('J')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("U") PORT_CODE(KEYCODE_U) PORT_CHAR('u') PORT_CHAR('U')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("7  \'") PORT_CODE(KEYCODE_7) PORT_CHAR('7') PORT_CHAR('\'')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F8") PORT_CODE(KEYCODE_F8) PORT_CHAR(UCHAR_MAMEKEY(F8))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Space") PORT_CODE(KEYCODE_SPACE) PORT_CHAR(' ')
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("N") PORT_CODE(KEYCODE_N) PORT_CHAR('n') PORT_CHAR('N')

	PORT_START("LINE1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("S") PORT_CODE(KEYCODE_S) PORT_CHAR('s') PORT_CHAR('S')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("W") PORT_CODE(KEYCODE_W) PORT_CHAR('w') PORT_CHAR('W')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("2  \"") PORT_CODE(KEYCODE_2) PORT_CHAR('2') PORT_CHAR('\"')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F3") PORT_CODE(KEYCODE_F3) PORT_CHAR(UCHAR_MAMEKEY(F3))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F22") //PORT_CODE(KEYCODE_SPACE)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F25")
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Z") PORT_CODE(KEYCODE_Z) PORT_CHAR('z') PORT_CHAR('Z')

	// 0x10
	PORT_START("LINE2")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("] }") PORT_CODE(KEYCODE_CLOSEBRACE) PORT_CHAR(']') PORT_CHAR('}')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("[ {") PORT_CODE(KEYCODE_OPENBRACE) PORT_CHAR('[') PORT_CHAR('{')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("^ ~") PORT_CODE(KEYCODE_TILDE) PORT_CHAR('^') PORT_CHAR('~')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F13")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F30")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Left") PORT_CODE(KEYCODE_LEFT) PORT_CHAR(UCHAR_MAMEKEY(LEFT))
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F28")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Right Shift") PORT_CODE(KEYCODE_RSHIFT) PORT_CHAR(UCHAR_SHIFT_1)

	PORT_START("LINE3")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("A") PORT_CODE(KEYCODE_A) PORT_CHAR('a') PORT_CHAR('A')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Q") PORT_CODE(KEYCODE_Q) PORT_CHAR('q') PORT_CHAR('Q')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("1 !") PORT_CODE(KEYCODE_1) PORT_CHAR('1') PORT_CHAR('!')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F2") PORT_CODE(KEYCODE_F2) PORT_CHAR(UCHAR_MAMEKEY(F2))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F23")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F26")
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Left Shift") PORT_CODE(KEYCODE_LSHIFT) PORT_CHAR(UCHAR_SHIFT_1)

	// 0x20
	PORT_START("LINE4")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("; +") PORT_CODE(KEYCODE_COLON) PORT_CHAR(';') PORT_CHAR('+')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("P") PORT_CODE(KEYCODE_P) PORT_CHAR('p') PORT_CHAR('P')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("0") PORT_CODE(KEYCODE_0) PORT_CHAR('0')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F11") PORT_CODE(KEYCODE_F11) PORT_CHAR(UCHAR_MAMEKEY(F11))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F31")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Right") PORT_CODE(KEYCODE_RIGHT) PORT_CHAR(UCHAR_MAMEKEY(RIGHT))
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F29")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME(". >")  PORT_CODE(KEYCODE_STOP) PORT_CHAR('.') PORT_CHAR('>')

	PORT_START("LINE5")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Shift Lock") PORT_CODE(KEYCODE_LALT)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Cntrl") PORT_CODE(KEYCODE_LCONTROL) PORT_CHAR(UCHAR_SHIFT_2)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Esc") PORT_CODE(KEYCODE_ESC) PORT_CHAR(27)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F1") PORT_CODE(KEYCODE_F1) PORT_CHAR(UCHAR_MAMEKEY(F1))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F2") PORT_CODE(KEYCODE_F2) PORT_CHAR(UCHAR_MAMEKEY(F2))
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F24")
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F27")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Caps Lock") PORT_CODE(KEYCODE_CAPSLOCK) PORT_CHAR(UCHAR_MAMEKEY(CAPSLOCK))

	// 0x30
	PORT_START("LINE6")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME(": *") PORT_CODE(KEYCODE_QUOTE) PORT_CHAR(':') PORT_CHAR('*')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("@ \\") PORT_CHAR('@') PORT_CHAR('\\')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("- =") PORT_CODE(KEYCODE_MINUS) PORT_CHAR('-') PORT_CHAR('=')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F12") PORT_CODE(KEYCODE_F12) PORT_CHAR(UCHAR_MAMEKEY(F12))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Down") PORT_CODE(KEYCODE_DOWN) PORT_CHAR(UCHAR_MAMEKEY(DOWN))
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Home") PORT_CODE(KEYCODE_HOME) PORT_CHAR(UCHAR_MAMEKEY(HOME))
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Up")  PORT_CODE(KEYCODE_UP) PORT_CHAR(UCHAR_MAMEKEY(UP))
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("/ ?") PORT_CODE(KEYCODE_SLASH) PORT_CHAR('/') PORT_CHAR('?')

	PORT_START("LINE7")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("K") PORT_CODE(KEYCODE_K) PORT_CHAR('k') PORT_CHAR('K')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("I") PORT_CODE(KEYCODE_I) PORT_CHAR('i') PORT_CHAR('I')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("8 (") PORT_CODE(KEYCODE_8) PORT_CHAR('8') PORT_CHAR('(')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F9") PORT_CODE(KEYCODE_F9) PORT_CHAR(UCHAR_MAMEKEY(F9))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("M") PORT_CODE(KEYCODE_M) PORT_CHAR('m') PORT_CHAR('M')

	// 0x40
	PORT_START("LINE8")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Line Feed") PORT_CODE(KEYCODE_ENTER_PAD) PORT_CHAR(10)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Enter") PORT_CODE(KEYCODE_ENTER) PORT_CHAR(13)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Backspace") PORT_CODE(KEYCODE_BACKSPACE) PORT_CHAR(8)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F15")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 0") PORT_CODE(KEYCODE_0_PAD)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 1") PORT_CODE(KEYCODE_1_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 4") PORT_CODE(KEYCODE_4_PAD)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F32")

	PORT_START("LINE9")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("G") PORT_CODE(KEYCODE_G) PORT_CHAR('g') PORT_CHAR('G')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("T") PORT_CODE(KEYCODE_T) PORT_CHAR('t') PORT_CHAR('T')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("5 %") PORT_CODE(KEYCODE_5) PORT_CHAR('5') PORT_CHAR('%')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F6") PORT_CODE(KEYCODE_F6) PORT_CHAR(UCHAR_MAMEKEY(F6))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F19")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 8") PORT_CODE(KEYCODE_8_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("V") PORT_CODE(KEYCODE_V) PORT_CHAR('v') PORT_CHAR('V')

	// 0x50
	PORT_START("LINEA")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Tab") PORT_CODE(KEYCODE_TAB) PORT_CHAR(9)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F16")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F17")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 2") PORT_CODE(KEYCODE_2_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 5") PORT_CODE(KEYCODE_5_PAD)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("LINEB")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F") PORT_CODE(KEYCODE_F) PORT_CHAR('f') PORT_CHAR('F')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("R") PORT_CODE(KEYCODE_R) PORT_CHAR('r') PORT_CHAR('R')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("4 $") PORT_CODE(KEYCODE_4) PORT_CHAR('4') PORT_CHAR('$')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F5") PORT_CODE(KEYCODE_F5) PORT_CHAR(UCHAR_MAMEKEY(F5))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F20")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 9") PORT_CODE(KEYCODE_9_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("C") PORT_CODE(KEYCODE_C) PORT_CHAR('c') PORT_CHAR('C')

	// 0x60
	PORT_START("LINEC")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Del") PORT_CODE(KEYCODE_DEL) PORT_CHAR(UCHAR_MAMEKEY(DEL))
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("_") PORT_CHAR('_')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("\\ |") PORT_CODE(KEYCODE_BACKSLASH)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F14")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad .") PORT_CODE(KEYCODE_DEL_PAD)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 3") PORT_CODE(KEYCODE_3_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 6") PORT_CODE(KEYCODE_6_PAD)
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Rep")

	PORT_START("LINED")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("D") PORT_CODE(KEYCODE_D) PORT_CHAR('d') PORT_CHAR('D')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("E") PORT_CODE(KEYCODE_E) PORT_CHAR('e') PORT_CHAR('E')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("3 \xE2\x82\xA4") PORT_CODE(KEYCODE_3) PORT_CHAR('3')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F4") PORT_CODE(KEYCODE_F4) PORT_CHAR(UCHAR_MAMEKEY(F4))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F21")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("X") PORT_CODE(KEYCODE_X) PORT_CHAR('x') PORT_CHAR('X')

	// 0x70
	PORT_START("LINEE")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("L") PORT_CODE(KEYCODE_L) PORT_CHAR('l') PORT_CHAR('L')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("O") PORT_CODE(KEYCODE_O) PORT_CHAR('o') PORT_CHAR('O')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("9 )") PORT_CODE(KEYCODE_9) PORT_CHAR('9') PORT_CHAR(')')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F10") PORT_CODE(KEYCODE_F10) PORT_CHAR(UCHAR_MAMEKEY(F10))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME(", <") PORT_CODE(KEYCODE_COMMA) PORT_CHAR(',') PORT_CHAR('<')

	PORT_START("LINEF")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("H") PORT_CODE(KEYCODE_H) PORT_CHAR('h') PORT_CHAR('H')
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Y") PORT_CODE(KEYCODE_Y) PORT_CHAR('y') PORT_CHAR('Y')
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("6 &") PORT_CODE(KEYCODE_6) PORT_CHAR('6') PORT_CHAR('&')
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F7") PORT_CODE(KEYCODE_F7) PORT_CHAR(UCHAR_MAMEKEY(F7))
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("F18")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("Numpad 7") PORT_CODE(KEYCODE_7_PAD)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_KEYBOARD )  PORT_NAME("B") PORT_CODE(KEYCODE_B) PORT_CHAR('b') PORT_CHAR('B')

INPUT_PORTS_END

ioport_constructor octopus_keyboard_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(octopus_keyboard);
}

void octopus_keyboard_device::device_start()
{
	buffered_rs232_device::device_start();

	save_item(NAME(m_delay));
	save_item(NAME(m_repeat));
	save_item(NAME(m_enabled));
}

void octopus_keyboard_device::device_reset()
{
	buffered_rs232_device::device_reset();

	set_data_frame(1, 8, PARITY_NONE, STOP_BITS_1);
	set_rcv_rate(1200);
	set_tra_rate(9600);
	receive_register_reset();
	transmit_register_reset();

	m_enabled = 0;
	m_delay = 500;  // 3*100+200
	m_repeat = 110;  // 4^2*5+30

	stop_processing();
	reset_key_state();
	typematic_stop();
	clear_fifo();

	output_dcd(0);
	output_dsr(0);
	output_cts(0);
	output_rxd(1);

	start_processing(attotime::from_hz(9600));
}

void octopus_keyboard_device::device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr)
{
	device_matrix_keyboard_interface::device_timer(timer, id, param, ptr);
	buffered_rs232_device::device_timer(timer, id, param, ptr);
}


const device_type OCTOPUS_KEYBOARD = device_creator<octopus_keyboard_device>;


