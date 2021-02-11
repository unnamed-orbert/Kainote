//  Copyright (c) 2018-2020, Marcin Drob

//  Kainote is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  Kainote is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with Kainote.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "KaiDialog.h"
#include "KaiListCtrl.h"
#include "ListControls.h"
#include "MappedButton.h"
#include "Styles.h"

enum {
	TYPE_LIST = 10,
	ID_ADD_CATALOG = 1565,
	ID_REMOVE_CATALOG,
	ID_FONT_LIST
};

class StylePreview;


class CatalogList : public Item
{
public:
	CatalogList(const wxString &text/*, int _positionInTable*/) : Item(TYPE_LIST){
		name = text;
		modified = false;
		if (!catalogList)
			RefreshCatalogList();
	}
	virtual ~CatalogList(){
	};
	void OnMouseEvent(wxMouseEvent &event, bool enter, bool leave, KaiListCtrl *theList, Item **changed /* = NULL */);
	void OnPaint(wxMemoryDC *dc, int x, int y, int width, int height, KaiListCtrl *theList);
	Item* Copy(){ return new CatalogList(*this); }
	wxSize GetTextExtents(KaiListCtrl *theList);
	static void RefreshCatalogList();
private:
	bool enter = false;
	bool clicked = false;
	int lastX = 0;
	int lastY = 0;
	int lastWidth = 100;
	int lastHeight = 22;
	static PopupList* floatingList;
	static wxArrayString* catalogList;
};

class FontItem : public Item
{
public:
	FontItem(const wxString &text) : Item(TYPE_TEXT){
		name = text;
		modified = false;
	}
	virtual ~FontItem(){};
	wxSize GetTextExtents(KaiListCtrl *theList);
private:
	void OnMouseEvent(wxMouseEvent &event, bool enter, bool leave, KaiListCtrl *theList, Item **changed /* = NULL */);
	void OnPaint(wxMemoryDC *dc, int x, int y, int width, int height, KaiListCtrl *theList);
	Item* Copy(){ return new FontItem(*this); }
	bool enter = false;
};

class FontSample : public Item
{
public:
	FontSample(const wxString& text) : Item(TYPE_TEXT) {
		name = text;
		modified = false;
	}
	virtual ~FontSample() {};
private:
	void OnPaint(wxMemoryDC* dc, int x, int y, int width, int height, KaiListCtrl* theList);
	Item* Copy() { return new FontSample(*this); }
};


class FontCatalogList : public KaiDialog
{
public:
	FontCatalogList(wxWindow *parent);
	virtual ~FontCatalogList();
	void ClearList();
	void GenerateList();
	void RefreshPreview();
	
private:
	KaiChoice* catalog;
	//MappedButton *replaceChecked;
	KaiListCtrl* fontList;
	StylePreview* preview;
	Styles fontStyle;
};

typedef wxArrayString* fontList;

class FontCatalogManagement {

public:
	FontCatalogManagement() { LoadCatalogs(); };
	~FontCatalogManagement();
	void LoadCatalogs();
	void SaveCatalogs();
	void DestroyCatalogs();
	wxArrayString* GetCatalogNames();
	wxString FindCatalogByFont(const wxString& font);
	wxArrayString* GetCatalogFonts(const wxString& catalog);
	void AddCatalog(const wxString& catalog, std::map<wxString, fontList>::iterator *it = NULL);
	void RemoveCatalog(const wxString& catalog);
	void AddCatalogFont(const wxString& catalog, const wxString& font);
	void AddCatalogFonts(const wxString& catalog, const wxArrayString& fonts);
	void RemoveCatalogFont(const wxString& catalog, const wxString& font);
	void ReplaceCatalogFonts(const wxString& catalog, const wxArrayString& fonts);
	std::map<wxString, fontList>* GetCatalogsMap() { return &fontCatalogs; };

private:
	wxArrayString fontCatalogsNames;
	std::map<wxString, fontList> fontCatalogs;
	bool isInit = false;

};


extern FontCatalogManagement FCManagement;