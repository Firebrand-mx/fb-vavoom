//**************************************************************************
//**
//**	##   ##    ##    ##   ##   ####     ####   ###     ###
//**	##   ##  ##  ##  ##   ##  ##  ##   ##  ##  ####   ####
//**	 ## ##  ##    ##  ## ##  ##    ## ##    ## ## ## ## ##
//**	 ## ##  ########  ## ##  ##    ## ##    ## ##  ###  ##
//**	  ###   ##    ##   ###    ##  ##   ##  ##  ##       ##
//**	   #    ##    ##    #      ####     ####   ##       ##
//**
//**	$Id: r_tex.cpp 2915 2007-11-28 19:50:46Z dj_jl $
//**
//**	Copyright (C) 1999-2006 Jānis Legzdiņš
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
//**
//**	Preparation of data for rendering, generation of lookups.
//**
//**************************************************************************

// HEADER FILES ------------------------------------------------------------

#include "gamedefs.h"
#include "r_local.h"

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

// PUBLIC DATA DEFINITIONS -------------------------------------------------

//
//	Main palette
//
rgba_t				r_palette[256];
vuint8				r_black_colour;

extern "C" {
vuint8				r_rgbtable[32 * 32 * 32 + 4];
};

VTextureTranslation**			TranslationTables;
int								NumTranslationTables;
TArray<VTextureTranslation*>	DecorateTranslations;

// PRIVATE DATA DEFINITIONS ------------------------------------------------

// CODE --------------------------------------------------------------------

//==========================================================================
//
//	InitPalette
//
//==========================================================================

static void InitPalette()
{
	guard(InitPalette);
	//	We use colour 0 as transparent colour, so we must find an alternate
	// index for black colour. In Doom, Heretic and Strife there is another
	// black colour, in Hexen it's almost black.
	//	I think that originaly Doom uses colour 255 as transparent colour,
	// but utilites created by others uses the alternate black colour and
	// these graphics can contain pixels of colour 255.
	//	Heretic and Hexen also uses colour 255 as transparent, even more - in
	// colourmaps it's maped to colour 0. Posibly this can cause problems
	// with modified graphics.
	//	Strife uses colour 0 as transparent. I already had problems with fact
	// that colour 255 is normal colour, now there shouldn't be any problems.
	VStream* Strm = W_CreateLumpReaderName(NAME_playpal);
	check(Strm);
	rgba_t* pal = r_palette;
	int best_dist = 0x10000;
	for (int i = 0; i < 256; i++)
	{
		*Strm << pal[i].r
			<< pal[i].g
			<< pal[i].b;
		if (i == 0)
		{
			pal[i].a = 0;
		}
		else
		{
			pal[i].a = 255;
			int dist = pal[i].r * pal[i].r + pal[i].g * pal[i].g +
				pal[i].b * pal[i].b;
			if (dist < best_dist)
			{
				r_black_colour = i;
				best_dist = dist;
			}
		}
	}
	delete Strm;
	unguard;
}

//==========================================================================
//
//	InitRgbTable
//
//==========================================================================

static void InitRgbTable()
{
	guard(InitRgbTable);
	for (int ir = 0; ir < 32; ir++)
	{
		for (int ig = 0; ig < 32; ig++)
		{
			for (int ib = 0; ib < 32; ib++)
			{
				int r = (int)(ir * 255.0 / 31.0 + 0.5);
				int g = (int)(ig * 255.0 / 31.0 + 0.5);
				int b = (int)(ib * 255.0 / 31.0 + 0.5);
				int best_colour = 0;
				int best_dist = 0x1000000;
				for (int i = 1; i < 256; i++)
				{
					int dist = (r_palette[i].r - r) * (r_palette[i].r - r) +
						(r_palette[i].g - g) * (r_palette[i].g - g) +
						(r_palette[i].b - b) * (r_palette[i].b - b);
					if (dist < best_dist)
					{
						best_colour = i;
						best_dist = dist;
						if (!dist)
							break;
					}
				}
				r_rgbtable[(ir << 10) + (ig << 5) + ib] = best_colour;
			}
		}
	}
	r_rgbtable[32 * 32 * 32] = 0;
	unguard;
}

//==========================================================================
//
//	InitTranslationTables
//
//==========================================================================

static void InitTranslationTables()
{
	guard(InitTranslationTables);
	VStream* Strm = W_CreateLumpReaderName(NAME_translat);
	NumTranslationTables = Strm->TotalSize() / 256;
	TranslationTables = new VTextureTranslation*[NumTranslationTables];
	for (int j = 0; j < NumTranslationTables; j++)
	{
		VTextureTranslation* Trans = new VTextureTranslation;
		TranslationTables[j] = Trans;
		Strm->Serialise(Trans->Table, 256);
		//	Make sure that 0 always maps to 0.
		Trans->Table[0] = 0;
		Trans->Palette[0] = r_palette[0];
		for (int i = 1; i < 256; i++)
		{
			//	Make sure that normal colours doesn't map to colour 0.
			if (Trans->Table[i] == 0)
			{
				Trans->Table[i] = r_black_colour;
			}
			Trans->Palette[i] = r_palette[Trans->Table[i]];
		}
	}
	delete Strm;
	unguard;
}

//==========================================================================
//
//	R_InitData
//
//==========================================================================

void R_InitData()
{
	guard(R_InitData);
	//	Load palette.
	InitPalette();

	//	Calculate RGB table.
	InitRgbTable();

	//	Init standard translation tables.
	InitTranslationTables();
	unguard;
}

//==========================================================================
//
//	R_ShutdownData
//
//==========================================================================

void R_ShutdownData()
{
	guard(R_ShutdownData);
	if (TranslationTables)
	{
		for (int i = 0; i < NumTranslationTables; i++)
		{
			delete TranslationTables[i];
		}
		delete[] TranslationTables;
		TranslationTables = NULL;
	}

	for (int i = 0; i < DecorateTranslations.Num(); i++)
	{
		delete DecorateTranslations[i];
	}
	DecorateTranslations.Clear();
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::VTextureTranslation
//
//==========================================================================

VTextureTranslation::VTextureTranslation()
: Crc(0)
, TranslStart(0)
, TranslEnd(0)
, Colour(0)
{
	Clear();
}

//==========================================================================
//
//	VTextureTranslation::Clear
//
//==========================================================================

void VTextureTranslation::Clear()
{
	guard(VTextureTranslation::Clear);
	for (int i = 0; i < 256; i++)
	{
		Table[i] = i;
		Palette[i] = r_palette[i];
	}
	Commands.Clear();
	CalcCrc();
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::CalcCrc
//
//==========================================================================

void VTextureTranslation::CalcCrc()
{
	guard(VTextureTranslation::CalcCrc);
	TCRC Work;
	Work.Init();
	for (int i = 1; i < 256; i++)
	{
		Work + Palette[i].r;
		Work + Palette[i].g;
		Work + Palette[i].b;
	}
	Crc = Work;
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::Serialise
//
//==========================================================================

void VTextureTranslation::Serialise(VStream& Strm)
{
	guard(VTextureTranslation::Serialise);
	Strm.Serialise(Table, 256);
	Strm.Serialise(Palette, sizeof(Palette));
	Strm << Crc
		<< TranslStart
		<< TranslEnd
		<< Colour;
	int CmdsSize = Commands.Num();
	Strm << STRM_INDEX(CmdsSize);
	if (Strm.IsLoading())
	{
		Commands.SetNum(CmdsSize);
	}
	for (int i = 0; i < CmdsSize; i++)
	{
		VTransCmd& C = Commands[i];
		Strm << C.Type << C.Start << C.End << C.R1 << C.G1 << C.B1 <<
			C.R2 << C.G2 << C.B2;
	}
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::BuildPlayerTrans
//
//==========================================================================

void VTextureTranslation::BuildPlayerTrans(int Start, int End, int Col)
{
	guard(VTextureTranslation::BuildPlayerTrans);
	int Count;
	Count = End - Start + 1;
	int r = (Col >> 16) & 0xff;
	int g = (Col >> 8) & 0xff;
	int b = Col & 0xff;
	bool Reverse = (r_palette[End].r + r_palette[End].g + r_palette[End].b) <
		(r_palette[Start].r + r_palette[Start].g + r_palette[Start].b);
	for (int i = 0; i < Count; i++)
	{
		int Idx = Start + i;
		int Mul = Reverse ? Count - i : i + 1;
		Palette[Idx].r = r * Mul / Count;
		Palette[Idx].g = g * Mul / Count;
		Palette[Idx].b = b * Mul / Count;
		Table[Idx] =r_rgbtable[((Palette[Idx].r << 7) & 0x7c00) +
			((Palette[Idx].g << 2) & 0x3e0) +
			((Palette[Idx].b >> 3) & 0x1f)];
	}
	CalcCrc();
	TranslStart = Start;
	TranslEnd = End;
	Colour = Col;
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::MapToRange
//
//==========================================================================

void VTextureTranslation::MapToRange(int AStart, int AEnd, int ASrcStart,
	int ASrcEnd)
{
	guard(VTextureTranslation::MapToRange);
	int Start;
	int End;
	int SrcStart;
	int SrcEnd;
	//	Swap range if necesary.
	if (AStart > AEnd)
	{
		Start = AEnd;
		End = AStart;
		SrcStart = ASrcEnd;
		SrcEnd = ASrcStart;
	}
	else
	{
		Start = AStart;
		End = AEnd;
		SrcStart = ASrcStart;
		SrcEnd = ASrcEnd;
	}
	//	Check for single colour change.
	if (Start == End)
	{
		Table[Start] = SrcStart;
		Palette[Start] = r_palette[SrcStart];
		return;
	}
	float CurCol = SrcStart;
	float ColStep = (float(SrcEnd) - float(SrcStart)) /
		(float(End) - float(Start));
	for (int i = Start; i < End; i++, CurCol += ColStep)
	{
		Table[i] = int(CurCol);
		Palette[i] = r_palette[Table[i]];
	}
	VTransCmd& C = Commands.Alloc();
	C.Type = 0;
	C.Start = Start;
	C.End = End;
	C.R1 = SrcStart;
	C.R2 = SrcEnd;
	CalcCrc();
	unguard;
}

//==========================================================================
//
//	VTextureTranslation::MapToColours
//
//==========================================================================

void VTextureTranslation::MapToColours(int AStart, int AEnd, int AR1, int AG1,
	int AB1, int AR2, int AG2, int AB2)
{
	guard(VTextureTranslation::MapToColours);
	int Start;
	int End;
	int R1;
	int G1;
	int B1;
	int R2;
	int G2;
	int B2;
	//	Swap range if necesary.
	if (AStart > AEnd)
	{
		Start = AEnd;
		End = AStart;
		R1 = AR2;
		G1 = AG2;
		B1 = AB2;
		R2 = AR1;
		G2 = AG1;
		B2 = AB1;
	}
	else
	{
		Start = AStart;
		End = AEnd;
		R1 = AR1;
		G1 = AG1;
		B1 = AB1;
		R2 = AR2;
		G2 = AG2;
		B2 = AB2;
	}
	//	Check for single colour change.
	if (Start == End)
	{
		Palette[Start].r = R1;
		Palette[Start].g = G1;
		Palette[Start].b = B1;
		Table[Start] = r_rgbtable[((R1 << 7) & 0x7c00) + ((G1 << 2) & 0x3e0) +
			((B1 >> 3) & 0x1f)];
		return;
	}
	float CurR = R1;
	float CurG = G1;
	float CurB = B1;
	float RStep = (float(R2) - float(R1)) / (float(End) - float(Start));
	float GStep = (float(G2) - float(G1)) / (float(End) - float(Start));
	float BStep = (float(B2) - float(B1)) / (float(End) - float(Start));
	for (int i = Start; i < End; i++, CurR += RStep, CurG += GStep,
		CurB += BStep)
	{
		Palette[i].r = int(CurR);
		Palette[i].g = int(CurG);
		Palette[i].b = int(CurB);
		Table[i] = r_rgbtable[((Palette[i].r << 7) & 0x7c00) +
			((Palette[i].g << 2) & 0x3e0) + ((Palette[i].b >> 3) & 0x1f)];
	}
	VTransCmd& C = Commands.Alloc();
	C.Type = 1;
	C.Start = Start;
	C.End = End;
	C.R1 = R1;
	C.G1 = G1;
	C.B1 = B1;
	C.R2 = R2;
	C.G2 = G2;
	C.B2 = B2;
	CalcCrc();
	unguard;
}

//==========================================================================
//
//	CheckChar
//
//==========================================================================

static bool CheckChar(char*& pStr, char Chr)
{
	//	Skip whitespace
	while (*pStr && *pStr <= ' ')
	{
		pStr++;
	}
	if (*pStr != Chr)
	{
		return false;
	}
	pStr++;
	return true;
}

//==========================================================================
//
//	DoTransString
//
//==========================================================================

static void DoTransString(VTextureTranslation* Tr, const VStr& Str)
{
	guard(DoTransString);
	char* pStr = const_cast<char*>(*Str);

	//	Parse start and end of the range
	int Start = strtol(pStr, &pStr, 10);
	if (!CheckChar(pStr, ':'))
	{
		return;
	}
	int End = strtol(pStr, &pStr, 10);
	if (!CheckChar(pStr, '='))
	{
		return;
	}
	if (!CheckChar(pStr, '['))
	{
		int SrcStart = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ':'))
		{
			return;
		}
		int SrcEnd = strtol(pStr, &pStr, 10);
		Tr->MapToRange(Start, End, SrcStart, SrcEnd);
	}
	else
	{
		int R1 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ','))
		{
			return;
		}
		int G1 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ','))
		{
			return;
		}
		int B1 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ']'))
		{
			return;
		}
		if (!CheckChar(pStr, ':'))
		{
			return;
		}
		if (!CheckChar(pStr, '['))
		{
			return;
		}
		int R2 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ','))
		{
			return;
		}
		int G2 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ','))
		{
			return;
		}
		int B2 = strtol(pStr, &pStr, 10);
		if (!CheckChar(pStr, ']'))
		{
			return;
		}
		Tr->MapToColours(Start, End, R1, G1, B1, R2, G2, B2);
	}
	unguard;
}

//==========================================================================
//
//	R_ParseDecorateTranslation
//
//==========================================================================

int R_ParseDecorateTranslation(VScriptParser* sc)
{
	guard(R_ParseDecorateTranslation);
	//	First check for standard translation.
	if (sc->CheckNumber())
	{
		if (sc->Number < 0 || sc->Number >= NumTranslationTables - 1)
		{
			sc->Error(va("Translation must be in range [0, %d]",
				NumTranslationTables - 2));
		}
		return (TRANSL_Standard << TRANSL_TYPE_SHIFT) + sc->Number;
	}

	VTextureTranslation* Tr = new VTextureTranslation;

	do
	{
		sc->ExpectString();
		DoTransString(Tr, sc->String);
	}
	while (sc->Check(","));

	//	See if we already have this translation.
	for (int i = 0; i < DecorateTranslations.Num(); i++)
	{
		if (DecorateTranslations[i]->Crc != Tr->Crc)
		{
			continue;
		}
		if (memcmp(DecorateTranslations[i]->Palette, Tr->Palette,
			sizeof(Tr->Palette)))
		{
			continue;
		}
		//	Found a match.
		delete Tr;
		return (TRANSL_Decorate << TRANSL_TYPE_SHIFT) + i;
	}

	//	Add it.
	if (DecorateTranslations.Num() >= MAX_DECORATE_TRANSLATIONS)
	{
		sc->Error("Too many translations in DECORATE scripts");
	}
	DecorateTranslations.Append(Tr);
	return (TRANSL_Decorate << TRANSL_TYPE_SHIFT) +
		DecorateTranslations.Num() - 1;
	unguard;
}