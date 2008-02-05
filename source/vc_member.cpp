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

#ifdef IN_VCC
#include "../utils/vcc/vcc.h"
#else
#include "vc_local.h"
#endif

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

//==========================================================================
//
//	VProgsReader
//
//==========================================================================

class VProgsReader : public VStream
{
private:
	VStream*			Stream;

public:
	VName*				NameRemap;
	int					NumImports;
	VProgsImport*		Imports;
	int					NumExports;
	VProgsExport*		Exports;

	VProgsReader(VStream* InStream)
	: Stream(InStream)
	, NameRemap(0)
	, NumImports(0)
	, NumExports(0)
	, Exports(0)
	{
		bLoading = true;
	}
	~VProgsReader()
	{
		delete[] NameRemap;
		delete[] Imports;
		delete[] Exports;
		delete Stream;
	}

	//	Stream interface.
	void Serialise(void* Data, int Len)
	{
		Stream->Serialise(Data, Len);
	}
	void Seek(int Pos)
	{
		Stream->Seek(Pos);
	}
	int Tell()
	{
		return Stream->Tell();
	}
	int TotalSize()
	{
		return Stream->TotalSize();
	}
	bool AtEnd()
	{
		return Stream->AtEnd();
	}
	void Flush()
	{
		Stream->Flush();
	}
	bool Close()
	{
		return Stream->Close();
	}

	VStream& operator<<(VName& Name)
	{
		int NameIndex;
		*this << STRM_INDEX(NameIndex);
		Name = NameRemap[NameIndex];
		return *this;
	}
	VStream& operator<<(VMemberBase*& Ref)
	{
		int ObjIndex;
		*this << STRM_INDEX(ObjIndex);
		if (ObjIndex > 0)
		{
			check(ObjIndex <= NumExports);
			Ref = Exports[ObjIndex - 1].Obj;
		}
		else if (ObjIndex < 0)
		{
			check(-ObjIndex <= NumImports);
			Ref = Imports[-ObjIndex - 1].Obj;
		}
		else
		{
			Ref = NULL;
		}
		return *this;
	}

	VMemberBase* GetImport(int Index)
	{
		VProgsImport& I = Imports[Index];
		if (!I.Obj)
		{
			if (I.Type == MEMBER_Package)
				I.Obj = VMemberBase::StaticLoadPackage(I.Name, TLocation());
			else
				I.Obj = VMemberBase::StaticFindMember(I.Name,
					GetImport(-I.OuterIndex - 1), I.Type);
		}
		return I.Obj;
	}
	void ResolveImports()
	{
		for (int i = 0; i < NumImports; i++)
			GetImport(i);
	}
};

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

// PUBLIC DATA DEFINITIONS -------------------------------------------------

bool					VMemberBase::GObjInitialised;
TArray<VMemberBase*>	VMemberBase::GMembers;
VMemberBase*			VMemberBase::GMembersHash[4096];

TArray<VStr>			VMemberBase::GPackagePath;
TArray<VPackage*>		VMemberBase::GLoadedPackages;

VClass*					VMemberBase::GClasses;
TArray<VClass*>			VMemberBase::GNetClassLookup;

// PRIVATE DATA DEFINITIONS ------------------------------------------------

// CODE --------------------------------------------------------------------

//==========================================================================
//
//	VProgsImport::VProgsImport
//
//==========================================================================

VProgsImport::VProgsImport(VMemberBase* InObj, vint32 InOuterIndex)
: Type(InObj->MemberType)
, Name(InObj->Name)
, OuterIndex(InOuterIndex)
, Obj(InObj)
{
}

//==========================================================================
//
//	VProgsExport::VProgsExport
//
//==========================================================================

VProgsExport::VProgsExport(VMemberBase* InObj)
: Type(InObj->MemberType)
, Name(InObj->Name)
, Obj(InObj)
{
}

#ifndef IN_VCC

//==========================================================================
//
//	operator VStream << mobjinfo_t
//
//==========================================================================

VStream& operator<<(VStream& Strm, mobjinfo_t& MI)
{
	return Strm << STRM_INDEX(MI.doomednum)
		<< STRM_INDEX(MI.GameFilter)
		<< MI.class_id;
}

#endif

//==========================================================================
//
//	VMemberBase::VMemberBase
//
//==========================================================================

VMemberBase::VMemberBase(vuint8 AMemberType, VName AName, VMemberBase* AOuter,
	TLocation ALoc)
: MemberType(AMemberType)
, Name(AName)
, Outer(AOuter)
, Loc(ALoc)
{
	if (GObjInitialised)
	{
		MemberIndex = GMembers.Append(this);
		int HashIndex = Name.GetIndex() & 4095;
		HashNext = GMembersHash[HashIndex];
		GMembersHash[HashIndex] = this;
	}
}

//==========================================================================
//
//	VMemberBase::~VMemberBase
//
//==========================================================================

VMemberBase::~VMemberBase()
{
}

//==========================================================================
//
//	VMemberBase::GetFullName
//
//==========================================================================

VStr VMemberBase::GetFullName() const
{
	guardSlow(VMemberBase::GetFullName);
	if (Outer)
		return Outer->GetFullName() + "." + Name;
	return VStr(Name);
	unguardSlow;
}

//==========================================================================
//
//	VMemberBase::GetPackage
//
//==========================================================================

VPackage* VMemberBase::GetPackage() const
{
	guard(VMemberBase::GetPackage);
	for (const VMemberBase* p = this; p; p = p->Outer)
		if (p->MemberType == MEMBER_Package)
			return (VPackage*)p;
	Sys_Error("Member object %s not in a package", *GetFullName());
	return NULL;
	unguard;
}

//==========================================================================
//
//	VMemberBase::IsIn
//
//==========================================================================

bool VMemberBase::IsIn(VMemberBase* SomeOuter) const
{
	guardSlow(VMemberBase::IsIn);
	for (VMemberBase* Tst = Outer; Tst; Tst = Tst->Outer)
		if (Tst == SomeOuter)
			return true;
	return !SomeOuter;
	unguardSlow;
}

//==========================================================================
//
//	VMemberBase::Serialise
//
//==========================================================================

void VMemberBase::Serialise(VStream& Strm)
{
	Strm << Outer;
}

//==========================================================================
//
//	VMemberBase::PostLoad
//
//==========================================================================

void VMemberBase::PostLoad()
{
}

//==========================================================================
//
//	VMemberBase::Shutdown
//
//==========================================================================

void VMemberBase::Shutdown()
{
}

//==========================================================================
//
//	VMemberBase::StaticInit
//
//==========================================================================

void VMemberBase::StaticInit()
{
	guard(VMemberBase::StaticInit);
#ifndef IN_VCC
	for (VClass* C = GClasses; C; C = C->LinkNext)
	{
		C->MemberIndex = GMembers.Append(C);
		int HashIndex = C->Name.GetIndex() & 4095;
		C->HashNext = GMembersHash[HashIndex];
		GMembersHash[HashIndex] = C;
	}
#endif
	GObjInitialised = true;
	unguard;
}

//==========================================================================
//
//	VMemberBase::StaticExit
//
//==========================================================================

void VMemberBase::StaticExit()
{
	for (int i = 0; i < GMembers.Num(); i++)
	{
#ifndef IN_VCC
		if (GMembers[i]->MemberType != MEMBER_Class ||
			!(((VClass*)GMembers[i])->ObjectFlags & CLASSOF_Native))
#endif
		{
			delete GMembers[i];
		}
#ifndef IN_VCC
		else
		{
			GMembers[i]->Shutdown();
		}
#endif
	}
	GMembers.Clear();
	GLoadedPackages.Clear();
	GNetClassLookup.Clear();
#ifndef IN_VCC
	VClass::GMobjInfos.Clear();
	VClass::GScriptIds.Clear();
	VClass::GSpriteNames.Clear();
#endif
	GObjInitialised = false;
}

//==========================================================================
//
//	VMemberBase::StaticAddPackagePath
//
//==========================================================================

void VMemberBase::StaticAddPackagePath(const char* Path)
{
	GPackagePath.Append(Path);
}

//==========================================================================
//
//	VMemberBase::StaticLoadPackage
//
//==========================================================================

VPackage* VMemberBase::StaticLoadPackage(VName AName, TLocation l)
{
	guard(VMemberBase::StaticLoadPackage);
	int				i;
	VName*			NameRemap;
	dprograms_t		Progs;
#ifndef IN_VCC
	TCRC			crc;
#endif
	VProgsReader*	Reader;

	//	Check if already loaded.
	for (i = 0; i < GLoadedPackages.Num(); i++)
	{
		if (GLoadedPackages[i]->Name == AName)
		{
			return GLoadedPackages[i];
		}
	}

#ifdef IN_VCC
	dprintf("Loading package %s\n", *AName);

	//	Load PROGS from a specified file
	VStream* f = OpenFile(va("%s.dat", *AName));
	if (!f)
	{
		for (i = 0; i < GPackagePath.Num(); i++)
		{
			f = OpenFile(GPackagePath[i] + "/" + AName + ".dat");
			if (f)
				break;
		}
	}
	if (!f)
	{
		ParseError(l, "Can't find package %s", *AName);
		return NULL;
	}
	Reader = new VProgsReader(f);
#else
	if (fl_devmode && FL_FileExists(va("progs/%s.dat", *AName)))
	{
		//	Load PROGS from a specified file
		Reader = new VProgsReader(FL_OpenFileRead(va("progs/%s.dat", *AName)));
	}
	else
	{
		//	Load PROGS from wad file
		Reader = new VProgsReader(W_CreateLumpReaderName(AName, WADNS_Progs));
	}

	//	Calcutate CRC
	crc.Init();
	for (i = 0; i < Reader->TotalSize(); i++)
	{
		crc + Streamer<vuint8>(*Reader);
	}
#endif

	// Read the header
	Reader->Seek(0);
	Reader->Serialise(Progs.magic, 4);
	for (i = 1; i < (int)sizeof(Progs) / 4; i++)
	{
		*Reader << ((int*)&Progs)[i];
	}

	if (VStr::NCmp(Progs.magic, PROG_MAGIC, 4))
	{
		ParseError(l, "Package %s has wrong file ID", *AName);
		BailOut();
	}
	if (Progs.version != PROG_VERSION)
	{
		ParseError(l, "Package %s has wrong version number (%i should be %i)",
			*AName, Progs.version, PROG_VERSION);
		BailOut();
	}

	// Read names
	NameRemap = new VName[Progs.num_names];
	Reader->Seek(Progs.ofs_names);
	for (i = 0; i < Progs.num_names; i++)
	{
		VNameEntry E;
		*Reader << E;
		NameRemap[i] = E.Name;
	}
	Reader->NameRemap = NameRemap;

	Reader->Imports = new VProgsImport[Progs.num_imports];
	Reader->NumImports = Progs.num_imports;
	Reader->Seek(Progs.ofs_imports);
	for (i = 0; i < Progs.num_imports; i++)
	{
		*Reader << Reader->Imports[i];
	}
	Reader->ResolveImports();

	VProgsExport* Exports = new VProgsExport[Progs.num_exports];
	Reader->Exports = Exports;
	Reader->NumExports = Progs.num_exports;

	VPackage* Pkg = new VPackage(AName);
	GLoadedPackages.Append(Pkg);
#ifndef IN_VCC
	Pkg->Checksum = crc;
	Pkg->Reader = Reader;
#endif

	//	Create objects
	Reader->Seek(Progs.ofs_exportinfo);
	for (i = 0; i < Progs.num_exports; i++)
	{
		*Reader << Exports[i];
		switch (Exports[i].Type)
		{
		case MEMBER_Package:
			Exports[i].Obj = new VPackage(Exports[i].Name);
			break;
		case MEMBER_Field:
			Exports[i].Obj = new VField(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_Property:
			Exports[i].Obj = new VProperty(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_Method:
			Exports[i].Obj = new VMethod(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_State:
			Exports[i].Obj = new VState(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_Const:
			Exports[i].Obj = new VConstant(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_Struct:
			Exports[i].Obj = new VStruct(Exports[i].Name, NULL, TLocation());
			break;
		case MEMBER_Class:
#ifndef IN_VCC
			Exports[i].Obj = VClass::FindClass(*Exports[i].Name);
			if (!Exports[i].Obj)
#endif
			{
				Exports[i].Obj = new VClass(Exports[i].Name, NULL, TLocation());
			}
			break;
		}
	}

#ifndef IN_VCC
	//	Read strings.
	Pkg->Strings = new char[Progs.num_strings];
	Reader->Seek(Progs.ofs_strings);
	Reader->Serialise(Pkg->Strings, Progs.num_strings);
#endif

	//	Serialise objects.
	Reader->Seek(Progs.ofs_exportdata);
	for (i = 0; i < Progs.num_exports; i++)
	{
		Exports[i].Obj->Serialise(*Reader);
		if (!Exports[i].Obj->Outer)
			Exports[i].Obj->Outer = Pkg;
	}

#ifndef IN_VCC
	//	Set up info tables.
	Reader->Seek(Progs.ofs_mobjinfo);
	for (i = 0; i < Progs.num_mobjinfo; i++)
	{
		*Reader << VClass::GMobjInfos.Alloc();
	}
	Reader->Seek(Progs.ofs_scriptids);
	for (i = 0; i < Progs.num_scriptids; i++)
	{
		*Reader << VClass::GScriptIds.Alloc();
	}

	for (i = 0; i < Progs.num_exports; i++)
	{
		Exports[i].Obj->PostLoad();
	}

	//	Create default objects.
	for (i = 0; i < Progs.num_exports; i++)
	{
		if (Exports[i].Obj->MemberType == MEMBER_Class)
		{
			((VClass*)Exports[i].Obj)->CreateDefaults();
		}
	}

	if (AName == "engine")
	{
		for (VClass* Cls = GClasses; Cls; Cls = Cls->LinkNext)
		{
			if (!Cls->Outer)
			{
				Cls->PostLoad();
				Cls->CreateDefaults();
				Cls->Outer = Pkg;
			}
		}
	}
#endif

	delete Reader;
#ifndef IN_VCC
	Pkg->Reader = NULL;
#endif
	return Pkg;
	unguard;
}

//==========================================================================
//
//	VMemberBase::StaticFindMember
//
//==========================================================================

VMemberBase* VMemberBase::StaticFindMember(VName AName, VMemberBase* AOuter,
	vuint8 AType)
{
	guard(VMemberBase::StaticFindMember);
	int HashIndex = AName.GetIndex() & 4095;
	for (VMemberBase* m = GMembersHash[HashIndex]; m; m = m->HashNext)
	{
		if (m->Name == AName && (m->Outer == AOuter ||
			(AOuter == ANY_PACKAGE && m->Outer->MemberType == MEMBER_Package)) &&
			(AType == ANY_MEMBER || m->MemberType == AType))
		{
			return m;
		}
	}
	return NULL;
	unguard;
}

//==========================================================================
//
//	VMemberBase::StaticFindType
//
//==========================================================================

VFieldType VMemberBase::StaticFindType(VClass* AClass, VName Name)
{
	guard(VMemberBase::StaticFindType);
	if (Name == NAME_None)
	{
		return VFieldType(TYPE_Unknown);
	}

	VMemberBase* m = StaticFindMember(Name, ANY_PACKAGE, MEMBER_Class);
	if (m)
	{
		return VFieldType((VClass*)m);
	}
	m = StaticFindMember(Name, AClass ? (VMemberBase*)AClass :
		(VMemberBase*)ANY_PACKAGE, MEMBER_Struct);
	if (m)
	{
		return VFieldType((VStruct*)m);
	}
	if (AClass)
	{
		return StaticFindType(AClass->ParentClass, Name);
	}
	return VFieldType(TYPE_Unknown);
	unguard;
}

//==========================================================================
//
//	VMemberBase::StaticFindClass
//
//==========================================================================

VClass* VMemberBase::StaticFindClass(VName Name)
{
	guard(VMemberBase::StaticFindClass);
	VMemberBase* m = StaticFindMember(Name, ANY_PACKAGE, MEMBER_Class);
	if (m)
	{
		return (VClass*)m;
	}
	return NULL;
	unguard;
}

//==========================================================================
//
//	VMemberBase::StaticSetUpNetClasses
//
//==========================================================================

void VMemberBase::StaticSetUpNetClasses()
{
#ifndef IN_VCC
	guard(VMemberBase::StaticSetUpNetClasses);
	GNetClassLookup.Clear();
	GNetClassLookup.Append(NULL);
	for (int i = 0; i < GMembers.Num(); i++)
	{
		if (GMembers[i]->MemberType == MEMBER_Class)
		{
			VClass* C = static_cast<VClass*>(GMembers[i]);
			if (C->IsChildOf(VThinker::StaticClass()))
			{
				C->NetId = GNetClassLookup.Num();
				GNetClassLookup.Append(C);
			}
		}
	}
	unguard;
#endif
}