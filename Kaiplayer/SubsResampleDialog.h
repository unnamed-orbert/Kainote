//  Copyright (c) 2016, Marcin Drob

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


#include "KaiDialog.h"
#include "NumCtrl.h"
#include "ListControls.h"
#include "MappedButton.h"
#include "KaiRadioButton.h"


class SubsResampleDialog : public KaiDialog
{
public:
	SubsResampleDialog(wxWindow *parent, const wxSize &subsSize, const wxSize &videoSize, const wxString &subsMatrix, const wxString &videoMatrix);
	~SubsResampleDialog(){}
	NumCtrl *subsResolutionX;
	NumCtrl *subsResolutionY;
	NumCtrl *destinedResolutionX;
	NumCtrl *destinedResolutionY;
#ifdef whithMatrix
	KaiChoice *subsMatrix;
	KaiChoice *destinedMatrix;
#endif
	KaiRadioBox *resamplingOptions;
};

class SubsMismatchResolutionDialog : public KaiDialog
{
public:
	SubsMismatchResolutionDialog(wxWindow *parent, const wxSize &subsSize, const wxSize &videoSize);
	~SubsMismatchResolutionDialog(){}
	KaiRadioBox *resamplingOptions;
};