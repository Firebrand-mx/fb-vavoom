//**************************************************************************
//**
//**	##   ##    ##    ##   ##   ####     ####   ###     ###
//**	##   ##  ##  ##  ##   ##  ##  ##   ##  ##  ####   ####
//**	 ## ##  ##    ##  ## ##  ##    ## ##    ## ## ## ## ##
//**	 ## ##  ########  ## ##  ##    ## ##    ## ##  ###  ##
//**	  ###   ##    ##   ###    ##  ##   ##  ##  ##       ##
//**	   #    ##    ##    #      ####     ####   ##       ##
//**
//**	$Id$
//**
//**	Copyright (C) 1999-2001 J�nis Legzdi��
//**
//**	This program is free software; you can redistribute it and/or
//**  modify it under the terms of the GNU General Public License
//**  as published by the Free Software Foundation; either version 2
//**  of the License, or (at your option) any later version.
//**
//**	This program is distributed in the hope that it will be useful,
//**  but WITHOUT ANY WARRANTY; without even the implied warranty of
//**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//**  GNU General Public License for more details.
//**
//**************************************************************************

// HEADER FILES ------------------------------------------------------------

// MACROS ------------------------------------------------------------------

#define SCANCODECOUNT	(KEY_COUNT - 0x80)

// TYPES -------------------------------------------------------------------

//	Keys and buttons
enum
{
	K_UPARROW = 0x80,
	K_LEFTARROW,
	K_RIGHTARROW,
	K_DOWNARROW,
	K_INSERT,
	K_DELETE,
	K_HOME,
	K_END,
	K_PAGEUP,
	K_PAGEDOWN,

	K_PAD0,
	K_PAD1,
	K_PAD2,
	K_PAD3,
	K_PAD4,
	K_PAD5,
	K_PAD6,
	K_PAD7,
	K_PAD8,
	K_PAD9,

	K_NUMLOCK,
	K_PADDIVIDE,
	K_PADMULTIPLE,
	K_PADMINUS,
	K_PADPLUS,
	K_PADENTER,
	K_PADDOT,

	K_ESCAPE,
	K_ENTER,
	K_TAB,
	K_BACKSPACE,
	K_CAPSLOCK,

	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,

	K_LSHIFT,
	K_RSHIFT,
	K_LCTRL,
	K_RCTRL,
	K_LALT,
	K_RALT,

	K_LWIN,
	K_RWIN,
	K_MENU,

	K_PRINTSCRN,
	K_SCROLLLOCK,
	K_PAUSE,

	K_ABNT_C1,
	K_YEN,
	K_KANA,
	K_CONVERT,
	K_NOCONVERT,
	K_AT,
	K_CIRCUMFLEX,
	K_COLON2,
	K_KANJI,

	K_MOUSE1,
	K_MOUSE2,
	K_MOUSE3,

	K_MOUSED1,
	K_MOUSED2,
	K_MOUSED3,

	K_JOY1,
	K_JOY2,
	K_JOY3,
	K_JOY4,
	K_JOY5,
	K_JOY6,
	K_JOY7,
	K_JOY8,
	K_JOY9,
	K_JOY10,
	K_JOY11,
	K_JOY12,
	K_JOY13,
	K_JOY14,
	K_JOY15,
	K_JOY16,

	KEY_COUNT
};

//	Input event types.
enum evtype_t
{
	ev_keydown,
	ev_keyup,
	ev_mouse,
	ev_joystick
};

//	Event structure.
struct event_t
{
	evtype_t	type;		// event type
	int			data1;		// keys / mouse/joystick buttons
	int			data2;		// mouse/joystick x move
	int			data3;		// mouse/joystick y move
};

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

//	In system module
void IN_Init(void);
void IN_ReadInput(void);
void IN_Shutdown(void);

//	In main module
void IN_PostEvent(event_t* ev);
void IN_KeyEvent(int key, int press);
void IN_ProcessEvents(void);
int IN_ReadKey(void);

void IN_GetBindingKeys(const char *, int &, int &);
void IN_GetBindingKeyNames(const char *, char *, char *);
void IN_SetBinding(int, const char *, const char *);
void IN_WriteBindings(ostream &strm);

// PUBLIC DATA DECLARATIONS ------------------------------------------------

extern int		shiftdown;
extern int		ctrldown;
extern int		altdown;

//**************************************************************************
//
//	$Log$
//	Revision 1.5  2001/10/04 17:20:25  dj_jl
//	Saving config using streams
//
//	Revision 1.4  2001/08/31 17:24:52  dj_jl
//	Added some new keys
//	
//	Revision 1.3  2001/07/31 17:16:30  dj_jl
//	Just moved Log to the end of file
//	
//	Revision 1.2  2001/07/27 14:27:54  dj_jl
//	Update with Id-s and Log-s, some fixes
//
//**************************************************************************
