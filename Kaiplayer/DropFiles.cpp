﻿//  Copyright (c) 2016, Marcin Drob

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


#include "DropFiles.h"
#include "kainoteMain.h"

DragnDrop::DragnDrop(kainoteFrame* kfparent)
{
	Kai=kfparent;
}

bool DragnDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if(filenames.size()>1){
		Kai->OpenFiles(filenames);}
	else if(filenames.size()>0){
		wxString ext = filenames[0].AfterLast('.').Lower();
		int w,h;
		Kai->GetClientSize(&w,&h);
		h -= (Kai->Menubar->GetSize().y);
		if(y >= h && y <= h + Kai->Tabs->GetHeight() && ext != "lua" && ext != "moon"){
			int pixels;
			int tab = Kai->Tabs->FindTab(x-iconsize,&pixels);
			if(tab<0){Kai->InsertTab();}
			else if(Kai->Tabs->iter != tab){Kai->Tabs->ChangePage(tab);}
		}
		Kai->OpenFile(filenames[0]);
	}
	return true;
}
