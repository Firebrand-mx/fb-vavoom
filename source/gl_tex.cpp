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
//**	Copyright (C) 1999-2002 J�nis Legzdi��
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

#include "gl_local.h"

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

// PUBLIC DATA DEFINITIONS -------------------------------------------------

// PRIVATE DATA DEFINITIONS ------------------------------------------------

static byte ptex[8][8] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
};

// CODE --------------------------------------------------------------------

//==========================================================================
//
//	VOpenGLDrawer::InitData
//
//==========================================================================

void VOpenGLDrawer::InitData(void)
{
	guard(VOpenGLDrawer::InitData);
	byte *pal = (byte*)W_CacheLumpName("PLAYPAL", PU_CACHE);
	for (int i = 0; i < 256; i++)
	{
		pal8_to24[i].r = *pal++;
		pal8_to24[i].g = *pal++;
		pal8_to24[i].b = *pal++;
		pal8_to24[i].a = 255;
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::InitTextures
//
//==========================================================================

void VOpenGLDrawer::InitTextures(void)
{
	guard(VOpenGLDrawer::InitTextures);
	//	Textures
	texture_id = (GLuint*)Z_Calloc(numtextures * 4);
	texture_sent = (bool*)Z_Calloc(numtextures);
	texture_iw = (float*)Z_Calloc(numtextures * 4);
	texture_ih = (float*)Z_Calloc(numtextures * 4);
	// 	Flats
	flat_id = (GLuint*)Z_Calloc(numflats * 4);
	flat_sent = (bool*)Z_Calloc(numflats);
	// 	Sky maps
	if (skymaps)
	{
		skymap_id = (GLuint*)Z_Calloc(numskymaps * 4);
		skymap_sent = (bool*)Z_Calloc(numskymaps);
	}
	// 	Sprite lumps
	sprite_id = (GLuint*)Z_Calloc(numspritelumps * 4);
	sprite_sent = (bool*)Z_Calloc(numspritelumps);
	spriteiw = (float*)Z_Calloc(numspritelumps * 4);
	spriteih = (float*)Z_Calloc(numspritelumps * 4);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GenerateTextures
//
//==========================================================================

void VOpenGLDrawer::GenerateTextures(void)
{
	guard(VOpenGLDrawer::GenerateTextures);
	int			i, j;
	rgba_t		pbuf[8][8];

	glGenTextures(numtextures, texture_id);
	glGenTextures(numflats, flat_id);
	if (numskymaps)
	{
		glGenTextures(numskymaps, skymap_id);
	}
	glGenTextures(numspritelumps, sprite_id);
	glGenTextures(MAX_TRANSLATED_SPRITES, trspr_id);
	glGenTextures(MAX_PICS, pic_id);
	glGenTextures(NUM_BLOCK_SURFS, lmap_id);
	glGenTextures(NUM_BLOCK_SURFS, addmap_id);
	glGenTextures(MAX_SKIN_CACHE, skin_id);
	glGenTextures(1, &particle_texture);

	FlushTextures();

	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			pbuf[j][i].r = 255;
			pbuf[j][i].g = 255;
			pbuf[j][i].b = 255;
			pbuf[j][i].a = byte(ptex[j][i] * 255);
		}
	}
	glBindTexture(GL_TEXTURE_2D, particle_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, pbuf);

	texturesGenerated = true;
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::FlushTextures
//
//==========================================================================

void VOpenGLDrawer::FlushTextures(void)
{
	guard(VOpenGLDrawer::FlushTextures);
	memset(texture_sent, 0, numtextures);
	memset(flat_sent, 0, numflats);
	memset(skymap_sent, 0, numskymaps);
	memset(sprite_sent, 0, numspritelumps);
	memset(trspr_sent, 0, MAX_TRANSLATED_SPRITES);
	memset(pic_sent, 0, MAX_PICS);
	memset(skin_name, 0, sizeof(skin_name));
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::DeleteTextures
//
//==========================================================================

void VOpenGLDrawer::DeleteTextures(void)
{
	guard(VOpenGLDrawer::DeleteTextures);
	if (texturesGenerated)
	{
		glDeleteTextures(numtextures, texture_id);
		glDeleteTextures(numflats, flat_id);
		if (numskymaps)
		{
			glDeleteTextures(numskymaps, skymap_id);
		}
		glDeleteTextures(numspritelumps, sprite_id);
		glDeleteTextures(MAX_TRANSLATED_SPRITES, trspr_id);
		glDeleteTextures(MAX_PICS, pic_id);
		glDeleteTextures(NUM_BLOCK_SURFS, lmap_id);
		glDeleteTextures(NUM_BLOCK_SURFS, addmap_id);
		glDeleteTextures(MAX_SKIN_CACHE, skin_id);
		glDeleteTextures(1, &particle_texture);
		texturesGenerated = false;
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::ToPowerOf2
//
//==========================================================================

int VOpenGLDrawer::ToPowerOf2(int val)
{
	int answer = 1;
	while (answer < val)
		answer <<= 1;
	return answer;
}

//==========================================================================
//
//	VOpenGLDrawer::DrawColumnInCache
//
// 	Clip and draw a column from a patch into a flat buffer.
//
//		column - column to draw
//		cache - buffer
//		originx, originy - position of column in the buffer
//		cachewidth, cacheheight - size of the cache
//
//==========================================================================

void VOpenGLDrawer::DrawColumnInCache(column_t* column, rgba_t* cache,
	int originx, int originy, int cachewidth, int cacheheight)
{
	guard(VOpenGLDrawer::DrawColumnInCache);
	int		count;
	int		position;
	byte*	source;
	rgba_t*	dest;
	int		top = -1;	//	DeepSea tall patches support

	// step through the posts in a column
	while (column->topdelta != 0xff)
	{
		if (column->topdelta <= top)
		{
			top += column->topdelta;
		}
		else
		{
			top = column->topdelta;
		}
		source = (byte *)column + 3;
		count = column->length;
		position = originy + top;

		//	Clip position
		if (position < 0)
		{
			count += position;
			source -= position;
			position = 0;
		}
		if (position + count > cacheheight)
		{
			count = cacheheight - position;
		}
		dest = cache + originx + position * cachewidth;

		while (count-- > 0)
		{
			*dest = pal8_to24[*source];
			source++;
			dest += cachewidth;
		}

		column = (column_t *)((byte *)column + column->length + 4);
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GenerateTexture
//
// 	Using the texture definition, the composite texture is created from the
// patches, and each column is cached.
//
//==========================================================================

void VOpenGLDrawer::GenerateTexture(int texnum)
{
	guard(VOpenGLDrawer::GenerateTexture);
	rgba_t*			block;
	texdef_t*		texture;
	texpatch_t*		patch;
	patch_t*		realpatch;
	int				x;
	int				x1;
	int				x2;
	int				i;
	column_t*		patchcol;

	texture = textures[texnum];

	char HighResName[80];
	sprintf(HighResName, "textures/walls/%s.png", texture->name);
	for (i = 0; HighResName[i]; i++)
		HighResName[i] = tolower(HighResName[i]);
	if (FL_FindFile(HighResName, NULL))
	{
		Mod_LoadSkin(HighResName, 0);
		if (SkinBPP == 8)
		{
			rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
			for (int i = 0; i < SkinWidth * SkinHeight; i++)
			{
				buf[i] = SkinPal[SkinData[i]];
			}
			UploadTexture(SkinWidth, SkinHeight, buf);
			Z_Free(buf);
		}
		else
		{
			UploadTexture(SkinWidth, SkinHeight, (rgba_t *)SkinData);
		}
		Z_Free(SkinData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		texture_iw[texnum] = 1.0 / float(texture->width);
		texture_ih[texnum] = 1.0 / float(texture->height);
		texture_sent[texnum] = true;
		return;
	}

	block = (rgba_t*)Z_Calloc(4 * texture->width * texture->height);

	// Composite the columns together.
	patch = texture->patches;

	for (i = 0; i < texture->patchcount; i++, patch++)
	{
		realpatch = (patch_t*)W_CacheLumpNum(patch->patch, PU_CACHE);
		x1 = patch->originx;
		x2 = x1 + LittleShort(realpatch->width);

		if (x1 < 0)
			x = 0;
		else
			x = x1;

		if (x2 > texture->width)
			x2 = texture->width;

		for ( ; x < x2; x++)
		{
			patchcol = (column_t *)((byte *)realpatch
					+ LittleLong(realpatch->columnofs[x - x1]));
			DrawColumnInCache(patchcol, block, x, patch->originy,
				texture->width, texture->height);
		}
	}

	UploadTexture(texture->width, texture->height, block);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Z_Free(block);
	texture_iw[texnum] = 1.0 / float(texture->width);
	texture_ih[texnum] = 1.0 / float(texture->height);
	texture_sent[texnum] = true;
	unguard;
}

//==========================================================================
//
// 	VOpenGLDrawer::SetTexture
//
//==========================================================================

void VOpenGLDrawer::SetTexture(int tex)
{
	guard(VOpenGLDrawer::SetTexture);
	if (tex & TEXF_FLAT)
	{
		SetFlat(tex);
		return;
	}

	tex = R_TextureAnimation(tex);

	glBindTexture(GL_TEXTURE_2D, texture_id[tex]);
	if (!texture_sent[tex])
	{
		GenerateTexture(tex);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
	tex_iw = texture_iw[tex];
	tex_ih = texture_ih[tex];
	unguard;
}

//==========================================================================
//
// 	VOpenGLDrawer::SetSkyTexture
//
//==========================================================================

void VOpenGLDrawer::SetSkyTexture(int tex, bool double_sky)
{
	guard(VOpenGLDrawer::SetSkyTexture);
	if (tex & TEXF_SKY_MAP)
	{
		tex &= ~TEXF_SKY_MAP;
		glBindTexture(GL_TEXTURE_2D, skymap_id[tex]);
		if (!skymap_sent[tex])
		{
			Mod_LoadSkin(skymaps[tex].name, 0);
			if (SkinBPP == 8)
			{
				rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
				for (int i = 0; i < SkinWidth * SkinHeight; i++)
				{
					buf[i] = SkinPal[SkinData[i]];
				}
				UploadTexture(SkinWidth, SkinHeight, buf);
				Z_Free(buf);
			}
			else
			{
				UploadTexture(SkinWidth, SkinHeight, (rgba_t *)SkinData);
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			Z_Free(SkinData);
			skymap_sent[tex] = true;
			skymaps[tex].width = SkinWidth;
			skymaps[tex].height = SkinHeight;
		}
		tex_iw = 1.0 / skymaps[tex].width;
		tex_ih = 1.0 / skymaps[tex].height;
	}
	else
	{
		rgba_t saved = pal8_to24[0];
		if (double_sky)
		{
			pal8_to24[0].a = 0;
		}
		SetTexture(tex);
		if (double_sky)
		{
			pal8_to24[0] = saved;
		}
	}
	// No mipmaping for sky
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GenerateFlat
//
//==========================================================================

void VOpenGLDrawer::GenerateFlat(int num)
{
	guard(VOpenGLDrawer::GenerateFlat);
	char HighResName[80];
	sprintf(HighResName, "textures/flats/%s.png", W_LumpName(flatlumps[num]));
	for (int j = 0; HighResName[j]; j++)
		HighResName[j] = tolower(HighResName[j]);
	if (FL_FindFile(HighResName, NULL))
	{
		Mod_LoadSkin(HighResName, 0);
		if (SkinBPP == 8)
		{
			rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
			for (int i = 0; i < SkinWidth * SkinHeight; i++)
			{
				buf[i] = SkinPal[SkinData[i]];
			}
			UploadTexture(SkinWidth, SkinHeight, buf);
			Z_Free(buf);
		}
		else
		{
			UploadTexture(SkinWidth, SkinHeight, (rgba_t *)SkinData);
		}
		Z_Free(SkinData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		flat_sent[num] = true;
		return;
	}

	rgba_t *block = (rgba_t*)Z_Malloc(4 * 64 * 64);
	byte *data = (byte*)W_CacheLumpNum(flatlumps[num], PU_CACHE);
	for (int i = 0; i < 64 * 64; i++)
	{
		block[i] = pal8_to24[data[i]];
	}
	UploadTexture(64, 64, block);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Z_Free(block);
	flat_sent[num] = true;
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::SetFlat
//
//==========================================================================

void VOpenGLDrawer::SetFlat(int num)
{
	guard(VOpenGLDrawer::SetFlat);
	num = R_TextureAnimation(num);
	num &= ~TEXF_FLAT;

	glBindTexture(GL_TEXTURE_2D, flat_id[num]);

	if (!flat_sent[num])
	{
		GenerateFlat(num);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
	tex_iw = 1.0 / 64.0;
	tex_ih = 1.0 / 64.0;
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GenerateSprite
//
//==========================================================================

void VOpenGLDrawer::GenerateSprite(int lump)
{
	guard(VOpenGLDrawer::GenerateSprite);
	patch_t	*patch = (patch_t*)W_CacheLumpNum(spritelumps[lump], PU_STATIC);

	int w = LittleShort(patch->width);
	int h = LittleShort(patch->height);
	spriteiw[lump] = 1.0 / (float)w;
	spriteih[lump] = 1.0 / (float)h;

	rgba_t *block = (rgba_t*)Z_Calloc(4 * w * h);

	for (int x = 0; x < w; x++)
	{
		column_t *column = (column_t *)((byte *)patch +
			LittleLong(patch->columnofs[x]));

		// step through the posts in a column
		int top = -1;	//	DeepSea tall patches support
		while (column->topdelta != 0xff)
		{
			if (column->topdelta <= top)
			{
				top += column->topdelta;
			}
			else
			{
				top = column->topdelta;
			}
			byte* source = (byte *)column + 3;
			rgba_t* dest = block + x + top * w;
			int count = column->length;

			while (count--)
			{
				*dest = pal8_to24[*source];
				source++;
				dest += w;
			}
			column = (column_t *)((byte *)column + column->length + 4);
		}
	}

	// Generate The Texture
	UploadTexture(w, h, block);
	sprite_sent[lump] = true;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	Z_Free(block);
	Z_ChangeTag(patch, PU_CACHE);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GenerateTranslatedSprite
//
//==========================================================================

void VOpenGLDrawer::GenerateTranslatedSprite(int lump, int slot, int translation)
{
	guard(VOpenGLDrawer::GenerateTranslatedSprite);
	patch_t	*patch = (patch_t*)W_CacheLumpNum(spritelumps[lump], PU_STATIC);

	int w = LittleShort(patch->width);
	int h = LittleShort(patch->height);
	trspriw[slot] = 1.0 / (float)w;
	trsprih[slot] = 1.0 / (float)h;

	rgba_t *block = (rgba_t*)Z_Calloc(4 * w * h);
	trspr_lump[slot] = lump;
	trspr_tnum[slot] = translation;
	trspr_sent[slot] = true;

	byte *trtab = translationtables + translation * 256;

	for (int x = 0; x < w; x++)
	{
		column_t *column = (column_t *)((byte *)patch +
			LittleLong(patch->columnofs[x]));

		// step through the posts in a column
		int top = -1;	//	DeepSea tall patches support
		while (column->topdelta != 0xff)
		{
			if (column->topdelta <= top)
			{
				top += column->topdelta;
			}
			else
			{
				top = column->topdelta;
			}
			byte* source = (byte *)column + 3;
			rgba_t* dest = block + x + top * w;
			int count = column->length;

			while (count--)
			{
				*dest = pal8_to24[trtab[*source]];
				source++;
				dest += w;
			}
			column = (column_t *)((byte *)column + column->length + 4);
		}
	}

	// Generate The Texture
	UploadTexture(w, h, block);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	Z_Free(block);
	Z_ChangeTag(patch, PU_CACHE);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::SetSpriteLump
//
//==========================================================================

void VOpenGLDrawer::SetSpriteLump(int lump, int translation)
{
	guard(VOpenGLDrawer::SetSpriteLump);
	if (translation)
	{
		int i;
		int avail = -1;
		for (i = 0; i <	MAX_TRANSLATED_SPRITES; i++)
		{
			if (trspr_sent[i])
			{
				if (trspr_lump[i] == lump && trspr_tnum[i] == translation)
				{
					glBindTexture(GL_TEXTURE_2D, trspr_id[i]);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
					tex_iw = trspriw[i];
					tex_ih = trsprih[i];
					return;
				}
			}
			else
			{
				if (avail < 0)
					avail = i;
			}
		}
		if (avail < 0)
		{
			avail = 0;
		}
		glBindTexture(GL_TEXTURE_2D, trspr_id[avail]);
		GenerateTranslatedSprite(lump, avail, translation);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
		tex_iw = trspriw[avail];
		tex_ih = trsprih[avail];
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, sprite_id[lump]);
		if (!sprite_sent[lump])
		{
			GenerateSprite(lump);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
		tex_iw = spriteiw[lump];
		tex_ih = spriteih[lump];
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::SetPic
//
//==========================================================================

void VOpenGLDrawer::SetPic(int handle)
{
	guard(VOpenGLDrawer::SetPic);
	glBindTexture(GL_TEXTURE_2D, pic_id[handle]);

	if (!pic_sent[handle])
	{
		switch (pic_list[handle].type)
 		{
	 	 case PIC_PATCH:
			GeneratePicFromPatch(handle);
			break;

		 case PIC_RAW:
			GeneratePicFromRaw(handle);
			break;
		}
	}

	tex_iw = pic_iw[handle];
	tex_ih = pic_ih[handle];

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GeneratePicFromPatch
//
//==========================================================================

void VOpenGLDrawer::GeneratePicFromPatch(int handle)
{
	guard(VOpenGLDrawer::GeneratePicFromPatch);
	int LumpNum = W_CheckNumForName(pic_list[handle].name);
	//	Some inventory pics are inside sprites.
	if (LumpNum < 0)
		LumpNum = W_GetNumForName(pic_list[handle].name, WADNS_Sprites);
	patch_t *patch = (patch_t*)W_CacheLumpNum(LumpNum, PU_STATIC);
	int w = LittleShort(patch->width);
	int h = LittleShort(patch->height);

	char HighResName[80];
	sprintf(HighResName, "textures/patches/%s.png", pic_list[handle].name);
	for (int i = 0; HighResName[i]; i++)
		HighResName[i] = tolower(HighResName[i]);
	if (FL_FindFile(HighResName, NULL))
	{
		Mod_LoadSkin(HighResName, 0);
		if (SkinBPP == 8)
		{
			rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
			for (int i = 0; i < SkinWidth * SkinHeight; i++)
			{
				buf[i] = SkinPal[SkinData[i]];
			}
			UploadTextureNoMip(SkinWidth, SkinHeight, buf);
			Z_Free(buf);
		}
		else
		{
			UploadTextureNoMip(SkinWidth, SkinHeight, (rgba_t *)SkinData);
		}
		Z_Free(SkinData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		pic_iw[handle] = 1.0 / float(w);
		pic_ih[handle] = 1.0 / float(h);
		pic_sent[handle] = true;
		Z_ChangeTag(patch, PU_CACHE);
		return;
	}

	rgba_t *block = (rgba_t*)Z_Calloc(4 * w * h);
	rgba_t *pal = r_palette[pic_list[handle].palnum];
	int black = r_black_color[pic_list[handle].palnum];

	for (int x = 0; x < w; x++)
	{
		column_t *column = (column_t *)((byte *)patch +
			LittleLong(patch->columnofs[x]));

		// step through the posts in a column
		int top = -1;	//	DeepSea tall patches support
		while (column->topdelta != 0xff)
		{
			if (column->topdelta <= top)
			{
				top += column->topdelta;
			}
			else
			{
				top = column->topdelta;
			}
			byte* source = (byte *)column + 3;
			rgba_t* dest = block + x + top * w;
			int count = column->length;

			while (count--)
			{
				*dest = pal[*source ? *source : black];
				source++;
				dest += w;
			}
			column = (column_t *)((byte *)column + column->length + 4);
		}
	}

	UploadTextureNoMip(w, h, block);
	Z_Free(block);
	Z_ChangeTag(patch, PU_CACHE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	pic_iw[handle] = 1.0 / float(w);
	pic_ih[handle] = 1.0 / float(h);
	pic_sent[handle] = true;
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::GeneratePicFromRaw
//
//==========================================================================

void VOpenGLDrawer::GeneratePicFromRaw(int handle)
{
	guard(VOpenGLDrawer::GeneratePicFromRaw);
	int lump = W_GetNumForName(pic_list[handle].name);
	int len = W_LumpLength(lump);
	int h = len / 320;

	char HighResName[80];
	sprintf(HighResName, "textures/patches/%s.png", pic_list[handle].name);
	for (int i = 0; HighResName[i]; i++)
		HighResName[i] = tolower(HighResName[i]);
	if (FL_FindFile(HighResName, NULL))
	{
		Mod_LoadSkin(HighResName, 0);
		if (SkinBPP == 8)
		{
			rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
			for (int i = 0; i < SkinWidth * SkinHeight; i++)
			{
				buf[i] = SkinPal[SkinData[i]];
			}
			UploadTextureNoMip(SkinWidth, SkinHeight, buf);
			Z_Free(buf);
		}
		else
		{
			UploadTextureNoMip(SkinWidth, SkinHeight, (rgba_t *)SkinData);
		}
		Z_Free(SkinData);
		if (h < 200)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		pic_iw[handle] = 1.0 / 320.0;
		pic_ih[handle] = 1.0 / float(h);
		pic_sent[handle] = true;
		return;
	}

	byte *raw = (byte*)W_CacheLumpNum(lump, PU_STATIC);
	rgba_t *block = (rgba_t*)Z_Calloc(4 * len);
	rgba_t *pal = r_palette[pic_list[handle].palnum];
	int black = r_black_color[pic_list[handle].palnum];

	byte *src = raw;
	rgba_t *dst = block;
	for (int i = 0; i < len; i++, src++, dst++)
	{
		*dst = pal[*src ? *src : black];
	}

	UploadTextureNoMip(320, h, block);
	Z_Free(block);
	Z_ChangeTag(raw, PU_CACHE);

	if (h < 200)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	pic_iw[handle] = 1.0 / float(320);
	pic_ih[handle] = 1.0 / float(h);
	pic_sent[handle] = true;
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::SetSkin
//
//==========================================================================

void VOpenGLDrawer::SetSkin(const char *name)
{
	guard(VOpenGLDrawer::SetSkin);
	int			i;
	int			avail;

	avail = -1;
	for (i = 0; i < MAX_SKIN_CACHE; i++)
	{
		if (skin_name[i][0])
		{
			if (!strcmp(skin_name[i], name))
			{
				glBindTexture(GL_TEXTURE_2D, skin_id[i]);
				break;
			}
		}
		else
		{
			if (avail < 0)
				avail = i;
		}
	}
	if (i == MAX_SKIN_CACHE)
	{
		// Not in cache, load it
		if (avail < 0)
		{
			avail = 0;
		}
		i = avail;
		glBindTexture(GL_TEXTURE_2D, skin_id[i]);
		strcpy(skin_name[i], name);
		Mod_LoadSkin(name, 0);
		if (SkinBPP == 8)
		{
			rgba_t *buf = (rgba_t*)Z_Malloc(SkinWidth * SkinHeight * 4);
			for (i = 0; i < SkinWidth * SkinHeight; i++)
			{
				buf[i] = SkinPal[SkinData[i]];
			}
			UploadTexture(SkinWidth, SkinHeight, buf);
			Z_Free(buf);
		}
		else
		{
			UploadTexture(SkinWidth, SkinHeight, (rgba_t *)SkinData);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		Z_Free(SkinData);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxfilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipfilter);
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::AdjustGamma
//
//==========================================================================

void VOpenGLDrawer::AdjustGamma(rgba_t *data, int size)
{
	guard(VOpenGLDrawer::AdjustGamma);
	byte *gt = gammatable[usegamma];
	for (int i = 0; i < size; i++)
	{
		data[i].r = gt[data[i].r];
		data[i].g = gt[data[i].g];
		data[i].b = gt[data[i].b];
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::ResampleTexture
//
//	Resizes	texture.
//	This is a simplified version of gluScaleImage from sources of MESA 3.0
//
//==========================================================================

void VOpenGLDrawer::ResampleTexture(int widthin, int heightin,
	const byte *datain, int widthout, int heightout, byte *dataout)
{
	guard(VOpenGLDrawer::ResampleTexture);
	int i, j, k;
	float sx, sy;

	if (widthout > 1)
		sx = float(widthin - 1) / float(widthout - 1);
	else
		sx = float(widthin - 1);
	if (heightout > 1)
		sy = float(heightin - 1) / float(heightout - 1);
	else
		sy = float(heightin - 1);

//#define POINT_SAMPLE
#ifdef POINT_SAMPLE
	for (i = 0; i < heightout; i++)
	{
		int ii = int(i * sy);
		for (j = 0; j < widthout; j++)
		{
			int jj = int(j * sx);

			const byte *src = datain + (ii * widthin + jj) * 4;
			byte *dst = dataout + (i * widthout + j) * 4;

			for (k = 0; k < 4; k++)
			{
				*dst++ = *src++;
			}
		}
	}
#else
	if (sx <= 1.0 && sy <= 1.0)
	{
		/* magnify both width and height:  use weighted sample of 4 pixels */
		int i0, i1, j0, j1;
		float alpha, beta;
		const byte *src00, *src01, *src10, *src11;
		float s1, s2;
		byte *dst;

		for (i = 0; i < heightout; i++)
		{
			i0 = int(i * sy);
			i1 = i0 + 1;
			if (i1 >= heightin) i1 = heightin-1;
			alpha = i * sy - i0;
			for (j = 0; j < widthout; j++)
			{
				j0 = int(j * sx);
				j1 = j0 + 1;
				if (j1 >= widthin) j1 = widthin-1;
				beta = j * sx - j0;

				/* compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
				src00 = datain + (i0 * widthin + j0) * 4;
				src01 = datain + (i0 * widthin + j1) * 4;
				src10 = datain + (i1 * widthin + j0) * 4;
				src11 = datain + (i1 * widthin + j1) * 4;

				dst = dataout + (i * widthout + j) * 4;

				for (k = 0; k < 4; k++)
				{
					s1 = *src00++ * (1.0-beta) + *src01++ * beta;
					s2 = *src10++ * (1.0-beta) + *src11++ * beta;
					*dst++ = byte(s1 * (1.0-alpha) + s2 * alpha);
				}
			}
		}
	}
	else
	{
		/* shrink width and/or height:  use an unweighted box filter */
		int i0, i1;
		int j0, j1;
		int ii, jj;
		int sum;
		byte *dst;

		for (i = 0; i < heightout; i++)
		{
			i0 = int(i * sy);
			i1 = i0 + 1;
			if (i1 >= heightin) i1 = heightin-1;
			for (j = 0; j < widthout; j++)
			{
				j0 = int(j * sx);
				j1 = j0 + 1;
				if (j1 >= widthin) j1 = widthin-1;

				dst = dataout + (i * widthout + j) * 4;

				/* compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
				for (k = 0; k < 4; k++)
				{
					sum = 0;
					for (ii = i0; ii <= i1; ii++)
					{
						for (jj = j0; jj <= j1; jj++)
						{
							sum += *(datain + (ii * widthin + jj) * 4 + k);
						}
					}
					sum /= (j1 - j0 + 1) * (i1 - i0 + 1);
					*dst++ = byte(sum);
				}
			}
		}
	}
#endif
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::MipMap
//
//	Scales image down for next mipmap level, operates in place
//
//==========================================================================

void VOpenGLDrawer::MipMap(int width, int height, byte *in)
{
	guard(VOpenGLDrawer::MipMap);
	int		i, j;
	byte	*out = in;

	if (width == 1 || height == 1)
	{
		//	Special case when only one dimension is scaled
		int total = width * height / 2;
		for (i = 0; i < total; i++, in += 8, out += 4)
		{
			out[0] = byte((in[0] + in[4]) >> 1);
			out[1] = byte((in[1] + in[5]) >> 1);
			out[2] = byte((in[2] + in[6]) >> 1);
			out[3] = byte((in[3] + in[7]) >> 1);
		}
		return;
	}

	//	Scale down in both dimensions
	width <<= 2;
	height >>= 1;
	for (i = 0; i < height; i++, in += width)
	{
		for (j = 0; j < width; j += 8, in += 8, out += 4)
		{
			out[0] = byte((in[0] + in[4] + in[width + 0] + in[width + 4]) >> 2);
			out[1] = byte((in[1] + in[5] + in[width + 1] + in[width + 5]) >> 2);
			out[2] = byte((in[2] + in[6] + in[width + 2] + in[width + 6]) >> 2);
			out[3] = byte((in[3] + in[7] + in[width + 3] + in[width + 7]) >> 2);
		}
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::UploadTexture
//
//==========================================================================

void VOpenGLDrawer::UploadTexture(int width, int height, rgba_t *data)
{
	guard(VOpenGLDrawer::UploadTexture);
	int		w, h;
	byte	*image;
	int		level;
	byte	stackbuf[256 * 128 * 4];

	AdjustGamma(data, width * height);

	w = ToPowerOf2(width);
	if (w > maxTexSize)
	{
		w = maxTexSize;
	}
	h = ToPowerOf2(height);
	if (h > maxTexSize)
	{
		h = maxTexSize;
	}

	if (w * h * 4 <= int(sizeof(stackbuf)))
	{
		image = stackbuf;
	}
	else
	{
		image = (byte*)Z_Malloc(w * h * 4, PU_HIGH, 0);
	}
	if (w != width || h != height)
	{
		/* must rescale image to get "top" mipmap texture image */
		ResampleTexture(width, height, (byte*)data, w, h, image);
	}
	else
	{
		memcpy(image, data, w * h * 4);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	for (level = 1; w > 1 || h > 1; level++)
	{
		MipMap(w, h, image);
		if (w > 1)
			w >>= 1;
		if (h > 1)
			h >>= 1;
		glTexImage2D(GL_TEXTURE_2D, level, 4, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, image);
	}

	if (image != stackbuf)
	{
		Z_Free(image);
	}
	unguard;
}

//==========================================================================
//
//	VOpenGLDrawer::UploadTextureNoMip
//
//==========================================================================

void VOpenGLDrawer::UploadTextureNoMip(int width, int height, rgba_t *data)
{
	guard(VOpenGLDrawer::UploadTextureNoMip);
	int		w, h;
	byte	*image;
	byte	stackbuf[64 * 1024];

	AdjustGamma(data, width * height);

	w = ToPowerOf2(width);
	if (w > maxTexSize)
	{
		w = maxTexSize;
	}
	h = ToPowerOf2(height);
	if (h > maxTexSize)
	{
		h = maxTexSize;
	}

	if (w != width || h != height)
	{
		/* must rescale image to get "top" mipmap texture image */
		if (w * h * 4 <= int(sizeof(stackbuf)))
		{
			image = stackbuf;
		}
		else
		{
			image = (byte*)Z_Malloc(w * h * 4, PU_HIGH, 0);
		}
		ResampleTexture(width, height, (byte*)data, w, h, image);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		if (image != stackbuf)
		{
			Z_Free(image);
		}
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	unguard;
}

//**************************************************************************
//
//	$Log$
//	Revision 1.21  2004/11/30 07:19:00  dj_jl
//	Support for high resolution textures.
//
//	Revision 1.20  2004/11/23 12:43:10  dj_jl
//	Wad file lump namespaces.
//	
//	Revision 1.19  2002/07/13 07:38:00  dj_jl
//	Added drawers to the object tree.
//	
//	Revision 1.18  2002/04/11 16:44:44  dj_jl
//	Got rid of some warnings.
//	
//	Revision 1.17  2002/03/20 19:09:53  dj_jl
//	DeepSea tall patches support.
//	
//	Revision 1.16  2002/01/11 18:24:44  dj_jl
//	Added guard macros
//	
//	Revision 1.15  2002/01/07 12:16:42  dj_jl
//	Changed copyright year
//	
//	Revision 1.14  2001/11/09 14:18:40  dj_jl
//	Added specular highlights
//	
//	Revision 1.13  2001/10/27 07:45:01  dj_jl
//	Added gamma controls
//	
//	Revision 1.12  2001/10/18 17:36:31  dj_jl
//	A lots of changes for Alpha 2
//	
//	Revision 1.11  2001/10/04 17:23:29  dj_jl
//	Got rid of some warnings
//	
//	Revision 1.10  2001/09/24 17:36:21  dj_jl
//	Added clamping
//	
//	Revision 1.9  2001/09/20 15:59:43  dj_jl
//	Fixed resampling when one dimansion doesn't change
//	
//	Revision 1.8  2001/08/30 17:37:39  dj_jl
//	Using linear texture resampling
//	
//	Revision 1.7  2001/08/24 17:05:44  dj_jl
//	Beautification
//	
//	Revision 1.6  2001/08/23 17:51:12  dj_jl
//	My own mipmap creation code, glu not used anymore
//	
//	Revision 1.5  2001/08/21 17:46:08  dj_jl
//	Added R_TextureAnimation, made SetTexture recognize flats
//	
//	Revision 1.4  2001/08/01 17:30:31  dj_jl
//	Fixed translated sprites
//	
//	Revision 1.3  2001/07/31 17:16:30  dj_jl
//	Just moved Log to the end of file
//	
//	Revision 1.2  2001/07/27 14:27:54  dj_jl
//	Update with Id-s and Log-s, some fixes
//
//**************************************************************************
