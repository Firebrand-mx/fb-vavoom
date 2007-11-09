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

struct VClipRect
{
	float		OriginX;	//	Origin of the widget, in absolute coordinates.
	float		OriginY;

	float		ScaleX;		//	Accomulative scale.
	float		ScaleY;

	float		ClipX1;		//	Clipping rectangle, in absolute coordinates.
	float		ClipY1;
	float		ClipX2;
	float		ClipY2;

	bool HasArea() const
	{
		return ClipX1 < ClipX2 && ClipY1 < ClipY2;
	}
};

class VWidget : public VObject
{
	DECLARE_CLASS(VWidget, VObject, 0)

private:
	//	Parent container widget.
	VWidget*			ParentWidget;
	//	Linked list of child widgets.
	VWidget*			FirstChildWidget;
	VWidget*			LastChildWidget;
	//	Links in the linked list of widgets.
	VWidget*			PrevWidget;
	VWidget*			NextWidget;

	//	Position of the widget in the parent widget.
	int					PosX;
	int					PosY;
	//	Size of the child area of the widget.
	int					SizeWidth;
	int					SizeHeight;
	//	Scaling of the widget.
	float				SizeScaleX;
	float				SizeScaleY;

	VClipRect			ClipRect;

	//	Currently focused child widget.
	VWidget*			CurrentFocusChild;

	// Booleans
	enum
	{
		//	Is this widget visible?
		WF_IsVisible		= 0x0001,
		//	A flag that enables or disables Tick event.
		WF_TickEnabled		= 0x0002,
		//	Is this widget enabled and can receive input.
		WF_IsEnabled		= 0x0004,
		//	Can this widget be focused?
		WF_IsFocusable		= 0x0008,
	};
	vuint32				WidgetFlags;

	VObjectDelegate 	FocusLost;
	VObjectDelegate 	FocusReceived;

	VObjectDelegate 	KeyDown;
	VObjectDelegate 	KeyUp;

	void AddChild(VWidget*);
	void RemoveChild(VWidget*);

	void ClipTree();
	void DrawTree();
	void TickTree(float DeltaTime);

	void FindNewFocus();

	bool TransferAndClipRect(float&, float&, float&, float&, float&, float&,
		float&, float&) const;

	friend class VRootWidget;

public:
	//	Destroys all child widgets.
	virtual void Init(VWidget*);
	void Destroy();
	void DestroyAllChildren();

	VRootWidget* GetRootWidget();

	//	Methods to move widget on top or bottom.
	void Lower();
	void Raise();

	//	Methods to set position, size and scale.
	void SetPos(int NewX, int NewY)
	{
		SetConfiguration(NewX, NewY, SizeWidth, SizeHeight, SizeScaleX,
			SizeScaleY);
	}
	void SetX(int NewX)
	{
		SetPos(NewX, PosY);
	}
	void SetY(int NewY)
	{
		SetPos(PosX, NewY);
	}
	void SetSize(int NewWidth, int NewHeight)
	{
		SetConfiguration(PosX, PosY, NewWidth, NewHeight, SizeScaleX,
			SizeScaleY);
	}
	void SetWidth(int NewWidth)
	{
		SetSize(NewWidth, SizeHeight);
	}
	void SetHeight(int NewHeight)
	{
		SetSize(SizeWidth, NewHeight);
	}
	void SetScale(float NewScaleX, float NewScaleY)
	{
		SetConfiguration(PosX, PosY, SizeWidth, SizeHeight, NewScaleX,
			NewScaleY);
	}
	void SetConfiguration(int, int, int, int, float = 1.0, float = 1.0);

	//	Visibility methods.
	void SetVisibility(bool);
	void Show()
	{
		SetVisibility(true);
	}
	void Hide()
	{
		SetVisibility(false);
	}
	bool IsVisible(bool bRecurse = true) const
	{
		if (bRecurse)
		{
			const VWidget* pParent = this;
			while (pParent)
			{
				if (!(pParent->WidgetFlags & WF_IsVisible))
				{
					break;
				}
				pParent = pParent->ParentWidget;
			}
			return (pParent ? false : true);
		}
		else
		{
			return !!(WidgetFlags & WF_IsVisible);
		}
	}

	//	Enable state methods
	void SetEnabled(bool);
	void Enable()
	{
		SetEnabled(true);
	}
	void Disable()
	{
		SetEnabled(false);
	}
	bool IsEnabled(bool bRecurse = true) const
	{
		if (bRecurse)
		{
			const VWidget* pParent = this;
			while (pParent)
			{
				if (!(pParent->WidgetFlags & WF_IsEnabled))
				{
					break;
				}
				pParent = pParent->ParentWidget;
			}
			return (pParent ? false : true);
		}
		else
		{
			return !!(WidgetFlags & WF_IsEnabled);
		}
	}

	//	Focusable state methods.
	void SetFocusable(bool);
	bool IsFocusable() const
	{
		return !!(WidgetFlags & WF_IsFocusable);
	}

	//	Focus methods.
	void SetCurrentFocusChild(VWidget*);
	VWidget* GetCurrentFocus() const
	{
		return CurrentFocusChild;
	}
	bool IsFocus(bool Recurse = true) const;
	void SetFocus();

	void OnCreate()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnCreate);
	}
	void OnDestroy()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnDestroy);
	}
	virtual void OnChildAdded(VWidget* Child)
	{
		P_PASS_SELF;
		P_PASS_REF(Child);
		EV_RET_VOID(NAME_OnChildAdded);
	}
	virtual void OnChildRemoved(VWidget* Child)
	{
		P_PASS_SELF;
		P_PASS_REF(Child);
		EV_RET_VOID(NAME_OnChildRemoved);
	}
	virtual void OnConfigurationChanged()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnConfigurationChanged);
	}
	virtual void OnVisibilityChanged(bool NewVisibility)
	{
		P_PASS_SELF;
		P_PASS_BOOL(NewVisibility);
		EV_RET_VOID(NAME_OnVisibilityChanged);
	}
	virtual void OnEnableChanged(bool bNewEnable)
	{
		P_PASS_SELF;
		P_PASS_BOOL(bNewEnable);
		EV_RET_VOID(NAME_OnEnableChanged);
	}
	virtual void OnFocusableChanged(bool bNewFocusable)
	{
		P_PASS_SELF;
		P_PASS_BOOL(bNewFocusable);
		EV_RET_VOID(NAME_OnFocusableChanged);
	}
	virtual void OnFocusReceived()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnFocusReceived);
	}
	virtual void OnFocusLost()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnFocusLost);
	}
	virtual void OnDraw()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnDraw);
	}
	virtual void OnPostDraw()
	{
		P_PASS_SELF;
		EV_RET_VOID(NAME_OnPostDraw);
	}
	virtual void Tick(float DeltaTime)
	{
		P_PASS_SELF;
		P_PASS_FLOAT(DeltaTime);
		EV_RET_VOID(NAME_Tick);
	}
	virtual bool OnKeyDown(int Key)
	{
		P_PASS_SELF;
		P_PASS_INT(Key);
		EV_RET_BOOL(NAME_OnKeyDown);
	}
	virtual bool OnKeyUp(int Key)
	{
		P_PASS_SELF;
		P_PASS_INT(Key);
		EV_RET_BOOL(NAME_OnKeyUp);
	}

	void DrawPic(int, int, int, float = 1.0);
	void DrawShadowedPic(int, int, int);
	void FillRectWithFlat(int, int, int, int, VName);
	void ShadeRect(int, int, int, int, float);

	void DrawString(int, int, const VStr&);
	void DrawText(int, int, const VStr&);
	int DrawTextW(int, int, const VStr&, int);

	static VWidget *CreateNewWidget(VClass*, VWidget*);

	DECLARE_FUNCTION(NewChild)
	DECLARE_FUNCTION(Destroy)
	DECLARE_FUNCTION(DestroyAllChildren)

	DECLARE_FUNCTION(GetRootWidget)

	DECLARE_FUNCTION(Raise)
	DECLARE_FUNCTION(Lower)

	DECLARE_FUNCTION(SetPos)
	DECLARE_FUNCTION(SetX)
	DECLARE_FUNCTION(SetY)
	DECLARE_FUNCTION(SetSize)
	DECLARE_FUNCTION(SetWidth)
	DECLARE_FUNCTION(SetHeight)
	DECLARE_FUNCTION(SetScale)
	DECLARE_FUNCTION(SetConfiguration)

	DECLARE_FUNCTION(SetVisibility)
	DECLARE_FUNCTION(Show)
	DECLARE_FUNCTION(Hide)
	DECLARE_FUNCTION(IsVisible)

	DECLARE_FUNCTION(SetEnabled)
	DECLARE_FUNCTION(Enable)
	DECLARE_FUNCTION(Disable)
	DECLARE_FUNCTION(IsEnabled)

	DECLARE_FUNCTION(SetFocusable)
	DECLARE_FUNCTION(IsFocusable)

	DECLARE_FUNCTION(SetCurrentFocusChild)
	DECLARE_FUNCTION(GetCurrentFocus)
	DECLARE_FUNCTION(IsFocus)
	DECLARE_FUNCTION(SetFocus)

	DECLARE_FUNCTION(DrawPic)
	DECLARE_FUNCTION(DrawShadowedPic)
	DECLARE_FUNCTION(FillRectWithFlat)
	DECLARE_FUNCTION(ShadeRect)

	DECLARE_FUNCTION(DrawText)
	DECLARE_FUNCTION(DrawTextW)
};