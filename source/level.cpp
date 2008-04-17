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

// HEADER FILES ------------------------------------------------------------

#include "gamedefs.h"
#include "sv_local.h"
#ifdef CLIENT
#include "cl_local.h"
#endif

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

// PUBLIC DATA DEFINITIONS -------------------------------------------------

IMPLEMENT_CLASS(V, Level);

VLevel*			GLevel;
VLevel*			GClLevel;

// PRIVATE DATA DEFINITIONS ------------------------------------------------

// CODE --------------------------------------------------------------------

//==========================================================================
//
//  VLevel::PointInSubsector
//
//==========================================================================

subsector_t* VLevel::PointInSubsector(const TVec &point) const
{
	guard(VLevel::PointInSubsector);
	node_t *node;
	int side;
	int nodenum;

	// single subsector is a special case
	if (!NumNodes)
		return Subsectors;

	nodenum = NumNodes - 1;

	while (!(nodenum & NF_SUBSECTOR))
	{
		node = &Nodes[nodenum];
		side = node->PointOnSide(point);
		nodenum = node->children[side];
	}
	return &Subsectors[nodenum & ~NF_SUBSECTOR];
	unguard;
}

//==========================================================================
//
//  VLevel::LeafPVS
//
//==========================================================================

byte *VLevel::LeafPVS(const subsector_t *ss) const
{
	guard(VLevel::LeafPVS);
	int sub = ss - Subsectors;
	if (VisData)
	{
		return VisData + (((NumSubsectors + 7) >> 3) * sub);
	}
	else
	{
		return NoVis;
	}
	unguard;
}

//==========================================================================
//
//	VLevel::Serialise
//
//==========================================================================

void VLevel::Serialise(VStream& Strm)
{
	guard(VLevel::Serialise);
	int i;
	int j;
	sector_t* sec;
	line_t* li;
	side_t* si;

	//
	//	Sectors
	//
	guard(Sectors);
	for (i = 0, sec = Sectors; i < NumSectors; i++, sec++)
	{
		Strm << sec->floor.dist
			<< sec->floor.TexZ
			<< sec->floor.pic
			<< sec->floor.xoffs
			<< sec->floor.yoffs
			<< sec->floor.XScale
			<< sec->floor.YScale
			<< sec->floor.Angle
			<< sec->floor.BaseAngle
			<< sec->floor.BaseYOffs
			<< sec->floor.flags
			<< sec->floor.Alpha
			<< sec->floor.MirrorAlpha
			<< sec->floor.LightSourceSector
			<< sec->floor.SkyBox
			<< sec->ceiling.dist
			<< sec->ceiling.TexZ
			<< sec->ceiling.pic
			<< sec->ceiling.xoffs
			<< sec->ceiling.yoffs
			<< sec->ceiling.XScale
			<< sec->ceiling.YScale
			<< sec->ceiling.Angle
			<< sec->ceiling.BaseAngle
			<< sec->ceiling.BaseYOffs
			<< sec->ceiling.flags
			<< sec->ceiling.Alpha
			<< sec->ceiling.MirrorAlpha
			<< sec->ceiling.LightSourceSector
			<< sec->ceiling.SkyBox
			<< sec->params.lightlevel
			<< sec->params.LightColour
			<< sec->params.Fade
			<< sec->params.contents
			<< sec->special
			<< sec->tag
			<< sec->seqType
			<< sec->SectorFlags
			<< sec->SoundTarget
			<< sec->FloorData
			<< sec->CeilingData
			<< sec->LightingData
			<< sec->AffectorData
			<< sec->ActionList
			<< sec->Damage
			<< sec->Friction
			<< sec->MoveFactor
			<< sec->Gravity
			<< sec->Sky;
		if (Strm.IsLoading())
		{
			CalcSecMinMaxs(sec);
			sec->ThingList = NULL;
		}
	}
	unguard;

	//
	//	Lines
	//
	guard(Lines);
	for (i = 0, li = Lines; i < NumLines; i++, li++)
	{
		//	Temporary hack to save seen on automap flags.
#ifdef CLIENT
		if (Strm.IsSaving() && cls.state == ca_connected)
		{
			li->flags |= GClLevel->Lines[i].flags & ML_MAPPED;
		}
#endif
		Strm << li->flags
			<< li->special
			<< li->arg1
			<< li->arg2
			<< li->arg3
			<< li->arg4
			<< li->arg5
			<< li->LineTag
			<< li->alpha;
		for (j = 0; j < 2; j++)
		{
			if (li->sidenum[j] == -1)
			{
				continue;
			}
			si = &Sides[li->sidenum[j]];
			Strm << si->textureoffset
				<< si->rowoffset
				<< si->toptexture
				<< si->bottomtexture
				<< si->midtexture
				<< si->Flags;
		}
	}
	unguard;

	//
	//	Polyobjs
	//
	guard(Polyobjs);
	for (i = 0; i < NumPolyObjs; i++)
	{
		if (Strm.IsLoading())
		{
			float angle, polyX, polyY;

			Strm << angle
				<< polyX 
				<< polyY;
			RotatePolyobj(PolyObjs[i].tag, angle);
			MovePolyobj(PolyObjs[i].tag,
				polyX - PolyObjs[i].startSpot.x,
				polyY - PolyObjs[i].startSpot.y);
		}
		else
		{
			Strm << PolyObjs[i].angle
				<< PolyObjs[i].startSpot.x
				<< PolyObjs[i].startSpot.y;
		}
		Strm << PolyObjs[i].SpecialData;
	}
	unguard;

	//
	//	Static lights
	//
	guard(StaticLights);
	Strm << STRM_INDEX(NumStaticLights);
	if (Strm.IsLoading())
	{
		if (StaticLights)
		{
			delete[] StaticLights;
			StaticLights = NULL;
		}
		if (NumStaticLights)
		{
			StaticLights = new rep_light_t[NumStaticLights];
		}
	}
	for (int i = 0; i < NumStaticLights; i++)
	{
		Strm << StaticLights[i].Origin
			<< StaticLights[i].Radius
			<< StaticLights[i].Colour;
	}
	unguard;

	//
	//	ACS
	//
	guard(ACS);
	Acs->Serialise(Strm);
	unguard;

	//
	//	Camera textures
	//
	guard(CameraTextures);
	int NumCamTex = CameraTextures.Num();
	Strm << STRM_INDEX(NumCamTex);
	if (Strm.IsLoading())
	{
		CameraTextures.SetNum(NumCamTex);
	}
	for (i = 0; i < NumCamTex; i++)
	{
		Strm << CameraTextures[i].Camera
			<< CameraTextures[i].TexNum
			<< CameraTextures[i].FOV;
	}
	unguard;

	//
	//	Translation tables
	//
	guard(Translations);
	int NumTrans = Translations.Num();
	Strm << STRM_INDEX(NumTrans);
	if (Strm.IsLoading())
	{
		Translations.SetNum(NumTrans);
	}
	for (i = 0; i < NumTrans; i++)
	{
		vuint8 Present = !!Translations[i];
		Strm << Present;
		if (Strm.IsLoading())
		{
			if (Present)
			{
				Translations[i] = new VTextureTranslation;
			}
			else
			{
				Translations[i] = NULL;
			}
		}
		if (Present)
		{
			Translations[i]->Serialise(Strm);
		}
	}
	unguard;

	//
	//	Body queue translation tables
	//
	guard(BoduQueueTranslations);
	int NumTrans = BodyQueueTrans.Num();
	Strm << STRM_INDEX(NumTrans);
	if (Strm.IsLoading())
	{
		BodyQueueTrans.SetNum(NumTrans);
	}
	for (i = 0; i < NumTrans; i++)
	{
		vuint8 Present = !!BodyQueueTrans[i];
		Strm << Present;
		if (Strm.IsLoading())
		{
			if (Present)
			{
				BodyQueueTrans[i] = new VTextureTranslation;
			}
			else
			{
				BodyQueueTrans[i] = NULL;
			}
		}
		if (Present)
		{
			BodyQueueTrans[i]->Serialise(Strm);
		}
	}
	unguard;
	unguard;
}

//==========================================================================
//
//	VLevel::ClearReferences
//
//==========================================================================

void VLevel::ClearReferences()
{
	guard(VLevel::ClearReferences);
	Super::ClearReferences();
	for (int i = 0; i < NumSectors; i++)
	{
		sector_t* sec = Sectors + i;
		if (sec->SoundTarget && sec->SoundTarget->GetFlags() & _OF_CleanupRef)
			sec->SoundTarget = NULL;
		if (sec->FloorData && sec->FloorData->GetFlags() & _OF_CleanupRef)
			sec->FloorData = NULL;
		if (sec->CeilingData && sec->CeilingData->GetFlags() & _OF_CleanupRef)
			sec->CeilingData = NULL;
		if (sec->LightingData && sec->LightingData->GetFlags() & _OF_CleanupRef)
			sec->LightingData = NULL;
		if (sec->AffectorData && sec->AffectorData->GetFlags() & _OF_CleanupRef)
			sec->AffectorData = NULL;
		if (sec->ActionList && sec->ActionList->GetFlags() & _OF_CleanupRef)
			sec->ActionList = NULL;
	}
	for (int i = 0; i < NumPolyObjs; i++)
	{
		if (PolyObjs[i].SpecialData && PolyObjs[i].SpecialData->GetFlags() & _OF_CleanupRef)
			PolyObjs[i].SpecialData = NULL;
	}
	for (int i = 0; i < CameraTextures.Num(); i++)
	{
		if (CameraTextures[i].Camera && CameraTextures[i].Camera->GetFlags() & _OF_CleanupRef)
			CameraTextures[i].Camera = NULL;
	}
	unguard;
}

//==========================================================================
//
//	VLevel::Destroy
//
//==========================================================================

void VLevel::Destroy()
{
	guard(VLevel::Destroy);
	//	Destroy all thinkers.
	DestroyAllThinkers();

	while (HeadSecNode)
	{
		msecnode_t* Node = HeadSecNode;
		HeadSecNode = Node->SNext;
		delete Node;
	}

	//	Free level data.
	if (RenderData)
	{
		delete RenderData;
	}

	for (int i = 0; i < NumPolyObjs; i++)
	{
		delete[] PolyObjs[i].segs;
		delete[] PolyObjs[i].originalPts;
		if (PolyObjs[i].prevPts)
		{
			delete[] PolyObjs[i].prevPts;
		}
	}
	if (PolyBlockMap)
	{
		for (int i = 0; i < BlockMapWidth * BlockMapHeight; i++)
		{
			for (polyblock_t* pb = PolyBlockMap[i]; pb;)
			{
				polyblock_t* Next = pb->next;
				delete pb;
				pb = Next;
			}
		}
		delete[] PolyBlockMap;
	}
	if (PolyObjs)
	{
		delete[] PolyObjs;
	}
	if (PolyAnchorPoints)
	{
		delete[] PolyAnchorPoints;
	}

	if (Sectors)
	{
		for (int i = 0; i < NumSectors; i++)
		{
			sec_region_t* Next;
			for (sec_region_t* r = Sectors[i].botregion; r; r = Next)
			{
				Next = r->next;
				delete r;
			}
		}
		delete[] Sectors[0].lines;
	}

	delete[] Vertexes;
	delete[] Sectors;
	delete[] Sides;
	delete[] Lines;
	delete[] Segs;
	delete[] Subsectors;
	delete[] Nodes;
	if (VisData)
	{
		delete[] VisData;
	}
	else
	{
		delete[] NoVis;
	}
	delete[] BlockMapLump;
	delete[] BlockLinks;
	delete[] RejectMatrix;
	delete[] Things;

	delete[] BaseLines;
	delete[] BaseSides;
	delete[] BaseSectors;
	delete[] BasePolyObjs;

	if (Acs)
	{
		delete Acs;
	}
	if (GenericSpeeches)
	{
		delete[] GenericSpeeches;
	}
	if (LevelSpeeches)
	{
		delete[] LevelSpeeches;
	}
	if (StaticLights)
	{
		delete[] StaticLights;
	}

	ActiveSequences.Clear();

	for (int i = 0; i < Translations.Num(); i++)
	{
		if (Translations[i])
		{
			delete Translations[i];
		}
	}
	Translations.Clear();
	for (int i = 0; i < BodyQueueTrans.Num(); i++)
	{
		if (BodyQueueTrans[i])
		{
			delete BodyQueueTrans[i];
		}
	}
	BodyQueueTrans.Clear();

	//	Call parent class's Destroy method.
	Super::Destroy();
	unguard;
}

//==========================================================================
//
//	VLevel::ClampOffsets
//
//==========================================================================

void VLevel::ClampOffsets()
{
	guard(VLevel::ClampOffsets);
	//	Clamp side offsets.
	for (int i = 0; i < NumSides; i++)
	{
		if (Sides[i].textureoffset > 0x7fff ||
			Sides[i].textureoffset < -0x8000)
		{
			Sides[i].textureoffset = 0;
		}
		if (Sides[i].rowoffset > 0x7fff || Sides[i].rowoffset < -0x8000)
		{
			Sides[i].rowoffset = 0;
		}
	}

	//	Clamp sector offsets.
	for (int i = 0; i < NumSectors; i++)
	{
		if (Sectors[i].floor.xoffs > 0x7fff ||
			Sectors[i].floor.xoffs < -0x8000)
		{
			Sectors[i].floor.xoffs = 0;
		}
		if (Sectors[i].floor.yoffs > 0x7fff ||
			Sectors[i].floor.yoffs < -0x8000)
		{
			Sectors[i].floor.yoffs = 0;
		}

		if (Sectors[i].ceiling.xoffs > 0x7fff ||
			Sectors[i].ceiling.xoffs < -0x8000)
		{
			Sectors[i].ceiling.xoffs = 0;
		}
		if (Sectors[i].ceiling.yoffs > 0x7fff ||
			Sectors[i].ceiling.yoffs < -0x8000)
		{
			Sectors[i].ceiling.yoffs = 0;
		}
	}
	unguard;
}

//==========================================================================
//
//  VLevel::SetCameraToTexture
//
//==========================================================================

void VLevel::SetCameraToTexture(VEntity* Ent, VName TexName, int FOV)
{
	guard(VLevel::SetCameraToTexture);
	if (!Ent)
	{
		return;
	}

	//	Get texture index.
	int TexNum = GTextureManager.CheckNumForName(TexName, TEXTYPE_Wall,
		true, false);
	if (TexNum < 0)
	{
		GCon->Logf("SetCameraToTexture: %s is not a valid texture",
			*TexName);
		return;
	}

	//	Make camera to be always relevant
	Ent->ThinkerFlags |= VEntity::TF_AlwaysRelevant;

	for (int i = 0; i < CameraTextures.Num(); i++)
	{
		if (CameraTextures[i].TexNum == TexNum)
		{
			CameraTextures[i].Camera = Ent;
			CameraTextures[i].FOV = FOV;
			return;
		}
	}
	VCameraTextureInfo& C = CameraTextures.Alloc();
	C.Camera = Ent;
	C.TexNum = TexNum;
	C.FOV = FOV;
	unguard;
}

//=============================================================================
//
//	VLevel::AddSecnode
//
// phares 3/16/98
//
// Searches the current list to see if this sector is
// already there. If not, it adds a sector node at the head of the list of
// sectors this object appears in. This is called when creating a list of
// nodes that will get linked in later. Returns a pointer to the new node.
//
//=============================================================================

msecnode_t* VLevel::AddSecnode(sector_t* Sec, VEntity* Thing,
	msecnode_t* NextNode)
{
	guard(VLevel::AddSecnode);
	msecnode_t* Node;

	if (!Sec)
	{
		Sys_Error("AddSecnode of 0 for %s\n", Thing->GetClass()->GetName());
	}

	Node = NextNode;
	while (Node)
	{
		if (Node->Sector == Sec)	// Already have a node for this sector?
		{
			Node->Thing = Thing;	// Yes. Setting m_thing says 'keep it'.
			return NextNode;
		}
		Node = Node->TNext;
	}

	// Couldn't find an existing node for this sector. Add one at the head
	// of the list.

	// Retrieve a node from the freelist.
	if (HeadSecNode)
	{
		Node = HeadSecNode;
		HeadSecNode = HeadSecNode->SNext;
	}
	else
	{
		Node = new msecnode_t;
	}

	// killough 4/4/98, 4/7/98: mark new nodes unvisited.
	//node->visited = 0;

	Node->Sector = Sec; 		// sector
	Node->Thing = Thing; 		// mobj
	Node->TPrev = NULL;			// prev node on Thing thread
	Node->TNext = NextNode;		// next node on Thing thread
	if (NextNode)
	{
		NextNode->TPrev = Node;	// set back link on Thing
	}

	// Add new node at head of sector thread starting at Sec->TouchingThingList

	Node->SPrev = NULL;			// prev node on sector thread
	Node->SNext = Sec->TouchingThingList; // next node on sector thread
	if (Sec->TouchingThingList)
	{
		Node->SNext->SPrev = Node;
	}
	Sec->TouchingThingList = Node;
	return Node;
	unguard;
}

//=============================================================================
//
//	VLevel::DelSecnode
//
//	Deletes a sector node from the list of sectors this object appears in.
// Returns a pointer to the next node on the linked list, or NULL.
//
//=============================================================================

msecnode_t* VLevel::DelSecnode(msecnode_t* Node)
{
	guard(VLevel::DelSecnode);
	msecnode_t*		tp;  // prev node on thing thread
	msecnode_t*		tn;  // next node on thing thread
	msecnode_t*		sp;  // prev node on sector thread
	msecnode_t*		sn;  // next node on sector thread

	if (Node)
	{
		// Unlink from the Thing thread. The Thing thread begins at
		// sector_list and not from VEntiy->TouchingSectorList.

		tp = Node->TPrev;
		tn = Node->TNext;
		if (tp)
		{
			tp->TNext = tn;
		}
		if (tn)
		{
			tn->TPrev = tp;
		}

		// Unlink from the sector thread. This thread begins at
		// sector_t->TouchingThingList.

		sp = Node->SPrev;
		sn = Node->SNext;
		if (sp)
		{
			sp->SNext = sn;
		}
		else
		{
			Node->Sector->TouchingThingList = sn;
		}
		if (sn)
		{
			sn->SPrev = sp;
		}

		// Return this node to the freelist

		Node->SNext = HeadSecNode;
		HeadSecNode = Node;
		return tn;
	}
	return NULL;
	unguard;
} 														// phares 3/13/98

//=============================================================================
//
//	VLevel::DelSectorList
//
//	Deletes the sector_list and NULLs it.
//
//=============================================================================

void VLevel::DelSectorList()
{
	guard(VLevel::DelSectorList);
	if (SectorList)
	{
		msecnode_t* Node = SectorList;
		while (Node)
		{
			Node = DelSecnode(Node);
		}
		SectorList = NULL;
	}
	unguard;
}

//==========================================================================
//
//  VLevel::SetBodyQueueTrans
//
//==========================================================================

int VLevel::SetBodyQueueTrans(int Slot, int Trans)
{
	guard(VLevel::SetBodyQueueTrans);
	int Type = Trans >> TRANSL_TYPE_SHIFT;
	int Index = Trans & ((1 << TRANSL_TYPE_SHIFT) - 1);
	if (Type != TRANSL_Player)
	{
		return Trans;
	}
	if (Slot < 0 || Slot > MAX_BODY_QUEUE_TRANSLATIONS || Index < 0 ||
		Index >= MAXPLAYERS || !LevelInfo->Game->Players[Index])
	{
		return Trans;
	}

	//	Add it.
	while (BodyQueueTrans.Num() <= Slot)
	{
		BodyQueueTrans.Append(NULL);
	}
	VTextureTranslation* Tr = BodyQueueTrans[Slot];
	if (!Tr)
	{
		Tr = new VTextureTranslation;
		BodyQueueTrans[Slot] = Tr;
	}
	Tr->Clear();
	VBasePlayer* P = LevelInfo->Game->Players[Index];
	Tr->BuildPlayerTrans(P->TranslStart, P->TranslEnd, P->Colour);
	return (TRANSL_BodyQueue << TRANSL_TYPE_SHIFT) + Slot;
	unguard;
}

//==========================================================================
//
//  CalcLine
//
//==========================================================================

void CalcLine(line_t *line)
{
	guard(CalcLine);
	//	Calc line's slopetype
	line->dir = *line->v2 - *line->v1;
	if (!line->dir.x)
	{
		line->slopetype = ST_VERTICAL;
	}
	else if (!line->dir.y)
	{
		line->slopetype = ST_HORIZONTAL;
	}
	else
	{
		if (line->dir.y / line->dir.x > 0)
		{
			line->slopetype = ST_POSITIVE;
		}
		else
		{
			line->slopetype = ST_NEGATIVE;
		}
	}

	line->SetPointDir(*line->v1, line->dir);

	//	Calc line's bounding box
	if (line->v1->x < line->v2->x)
	{
		line->bbox[BOXLEFT] = line->v1->x;
		line->bbox[BOXRIGHT] = line->v2->x;
	}
	else
	{
		line->bbox[BOXLEFT] = line->v2->x;
		line->bbox[BOXRIGHT] = line->v1->x;
	}
	if (line->v1->y < line->v2->y)
	{
		line->bbox[BOXBOTTOM] = line->v1->y;
		line->bbox[BOXTOP] = line->v2->y;
	}
	else
	{
		line->bbox[BOXBOTTOM] = line->v2->y;
		line->bbox[BOXTOP] = line->v1->y;
	}
	unguard;
}

//==========================================================================
//
//  CalcSeg
//
//==========================================================================

void CalcSeg(seg_t *seg)
{
	guardSlow(CalcSeg);
	seg->Set2Points(*seg->v1, *seg->v2);
	unguardSlow;
}

#ifdef SERVER

//==========================================================================
//
//  SV_LoadLevel
//
//==========================================================================

void SV_LoadLevel(VName MapName)
{
	guard(SV_LoadLevel);
#ifdef CLIENT
	if (GClLevel == GLevel)
	{
		GClLevel = NULL;
	}
#endif
	if (GLevel)
	{
		delete GLevel;
		GLevel = NULL;
	}

	GLevel = Spawn<VLevel>();
	GLevel->LevelFlags |= VLevel::LF_ForServer;

	GLevel->LoadMap(MapName);
	unguard;
}

#endif
#ifdef CLIENT

//==========================================================================
//
//	CL_LoadLevel
//
//==========================================================================

void CL_LoadLevel(VName MapName)
{
	guard(CL_LoadLevel);
	if (GClLevel)
	{
		delete GClLevel;
		GClLevel = NULL;
	}

	GClLevel = Spawn<VLevel>();
	GClGame->GLevel = GClLevel;

	GClLevel->LoadMap(MapName);
	unguard;
}

#endif

//==========================================================================
//
//  AddExtraFloor
//
//==========================================================================

sec_region_t *AddExtraFloor(line_t *line, sector_t *dst)
{
	guard(AddExtraFloor);
	sec_region_t *region;
	sec_region_t *inregion;
	sector_t *src;

	src = line->frontsector;
	src->SectorFlags |= sector_t::SF_ExtrafloorSource;
	dst->SectorFlags |= sector_t::SF_HasExtrafloors;
	float floorz = src->floor.GetPointZ(dst->soundorg);
	float ceilz = src->ceiling.GetPointZ(dst->soundorg);
	for (inregion = dst->botregion; inregion; inregion = inregion->next)
	{
		float infloorz = inregion->floor->GetPointZ(dst->soundorg);
		float inceilz = inregion->ceiling->GetPointZ(dst->soundorg);
		if (infloorz <= ceilz && inceilz >= floorz)
		{
			region = new sec_region_t;
			memset(region, 0, sizeof(*region));
			region->floor = inregion->floor;
			region->ceiling = &src->ceiling;
			region->params = &src->params;
			region->extraline = line;
			inregion->floor = &src->floor;
			if (inregion->prev)
			{
				inregion->prev->next = region;
			}
			else
			{
				dst->botregion = region;
			}
			region->prev = inregion->prev;
			region->next = inregion;
			inregion->prev = region;
			return region;
		}
	}
	GCon->Logf("Invalid extra floor, tag %d", dst->tag);
	return NULL;
	unguard;
}

//==========================================================================
//
//	SwapPlanes
//
//==========================================================================

void SwapPlanes(sector_t *s)
{
	guard(SwapPlanes);
	float tempHeight;
	int tempTexture;

	tempHeight = s->floorheight;
	tempTexture = s->floor.pic;

	//	Floor
	s->floorheight = s->ceilingheight;
	s->floor.dist = s->floorheight;
	s->floor.minz = s->floorheight;
	s->floor.maxz = s->floorheight;

	s->ceilingheight = tempHeight;
	s->ceiling.dist = -s->ceilingheight;
	s->ceiling.minz = s->ceilingheight;
	s->ceiling.maxz = s->ceilingheight;

	s->floor.pic = s->ceiling.pic;
	s->ceiling.pic = tempTexture;
	unguard;
}

//==========================================================================
//
//	CalcSecMinMaxs
//
//==========================================================================

void CalcSecMinMaxs(sector_t *sector)
{
	guard(CalcSecMinMaxs);
	float	minz;
	float	maxz;
	int		i;

	if (sector->floor.normal.z == 1.0)
	{
		//	Horisontal floor
		sector->floor.minz = sector->floor.dist;
		sector->floor.maxz = sector->floor.dist;
	}
	else
	{
		//	Sloped floor
		minz = 99999.0;
		maxz = -99999.0;
		for (i = 0; i < sector->linecount; i++)
		{
			float z;
			z = sector->floor.GetPointZ(*sector->lines[i]->v1);
			if (minz > z)
				minz = z;
			if (maxz < z)
				maxz = z;
		}
		sector->floor.minz = minz;
		sector->floor.maxz = maxz;
	}

	if (sector->ceiling.normal.z == -1.0)
	{
		//	Horisontal ceiling
		sector->ceiling.minz = -sector->ceiling.dist;
		sector->ceiling.maxz = -sector->ceiling.dist;
	}
	else
	{
		//	Sloped ceiling
		minz = 99999.0;
		maxz = -99999.0;
		for (i = 0; i < sector->linecount; i++)
		{
			float z;
			z = sector->ceiling.GetPointZ(*sector->lines[i]->v1);
			if (minz > z)
				minz = z;
			if (maxz < z)
				maxz = z;
		}
		sector->ceiling.minz = minz;
		sector->ceiling.maxz = maxz;
	}

	sector->floorheight = sector->floor.minz;
	sector->ceilingheight = sector->ceiling.maxz;
	unguard;
}

//==========================================================================
//
//	Natives
//
//==========================================================================

IMPLEMENT_FUNCTION(VLevel, PointInSector)
{
	P_GET_VEC(Point);
	P_GET_SELF;
	RET_PTR(Self->PointInSubsector(Point)->sector);
}

IMPLEMENT_FUNCTION(VLevel, ChangeSector)
{
	P_GET_INT(crunch);
	P_GET_PTR(sector_t, sec);
	P_GET_SELF;
	RET_BOOL(Self->ChangeSector(sec, crunch));
}

IMPLEMENT_FUNCTION(VLevel, AddExtraFloor)
{
	P_GET_PTR(sector_t, dst);
	P_GET_PTR(line_t, line);
	P_GET_SELF;
	RET_PTR(AddExtraFloor(line, dst));
}

IMPLEMENT_FUNCTION(VLevel, SwapPlanes)
{
	P_GET_PTR(sector_t, s);
	P_GET_SELF;
	SwapPlanes(s);
}

IMPLEMENT_FUNCTION(VLevel, SetFloorLightSector)
{
	P_GET_PTR(sector_t, SrcSector);
	P_GET_PTR(sector_t, Sector);
	P_GET_SELF;
	Sector->floor.LightSourceSector = SrcSector - Self->Sectors;
}

IMPLEMENT_FUNCTION(VLevel, SetCeilingLightSector)
{
	P_GET_PTR(sector_t, SrcSector);
	P_GET_PTR(sector_t, Sector);
	P_GET_SELF;
	Sector->ceiling.LightSourceSector = SrcSector - Self->Sectors;
}

IMPLEMENT_FUNCTION(VLevel, SetHeightSector)
{
	P_GET_INT(Flags);
	P_GET_PTR(sector_t, SrcSector);
	P_GET_PTR(sector_t, Sector);
	P_GET_SELF;
	if (Self->RenderData)
	{
		Self->RenderData->SetupFakeFloors(Sector);
	}
}

IMPLEMENT_FUNCTION(VLevel, FindSectorFromTag)
{
	P_GET_INT(start);
	P_GET_INT(tag);
	P_GET_SELF;
	int Ret = -1;
	for (int i = start + 1; i < Self->NumSectors; i++)
		if (Self->Sectors[i].tag == tag)
		{
			Ret = i;
			break;
		}
	RET_INT(Ret);
}

IMPLEMENT_FUNCTION(VLevel, SetBodyQueueTrans)
{
	P_GET_INT(Trans);
	P_GET_INT(Slot);
	P_GET_SELF;
	RET_INT(Self->SetBodyQueueTrans(Slot, Trans));
}
