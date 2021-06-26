//  Copyright (c) 2021, Marcin Drob

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
#include "ListControls.h"
#include "KaiTextCtrl.h"
#include <vector>
#include "Visuals.h"

class ShapesSetting
{
public:
	ShapesSetting() {};
	ShapesSetting(const wxString& _name, const wxString& _shape,
		unsigned char _mode = BOTH_SCALE_X, unsigned char _scalingMode = CHANGE_SCALE) {
		name = _name;
		shape = _shape;
		mode = _mode;
		scalingMode = _scalingMode;
	};
	ShapesSetting(const wxString& _name) {
		name = _name;
	};
	wxString name;
	wxString shape;

	unsigned char mode = 0;
	unsigned char scalingMode = 0;
	enum SHAPE_MODE{
		SEPARATE_SCALE_X_Y = 0,
		BOTH_SCALE_X,
		ONLY_SCALE_X
	};
	enum SCALING_MODE {
		CHANGE_POINTS = 0,
		CHANGE_SCALE,
	};
};

class ShapesEdition : public KaiDialog
{
public:
	ShapesEdition(wxWindow* parent, const wxPoint& pos, std::vector<ShapesSetting>* _shapes, int curShape);
	virtual ~ShapesEdition() {};
	std::vector<ShapesSetting>* GetShapes() { return &shapes; };
private:
	void OnSave(wxCommandEvent& evt);
	void OnResetDefault(wxCommandEvent& evt);
	void OnAddShape(wxCommandEvent& evt);
	void OnRemoveShape(wxCommandEvent& evt);
	void OnListChanged(wxCommandEvent& evt);
	void UpdateShape();
	void SetShapeFromSettings();
	void SetShape(int num);
	bool CheckModified();
	void Save(int id);
	enum {
		ID_SHAPE_LIST = 7809,
		ID_BUTTON_ADD_SHAPE,
		ID_BUTTON_REMOVE_SHAPE,
		ID_BUTTON_COMMIT,
		ID_BUTTON_OK,
		ID_BUTTON_RESET_DEFAULT
	};
	KaiChoice* shapeList;
	KaiChoice* mode;
	KaiChoice* scalingMode;
	KaiTextCtrl* newShapeName;
	KaiTextCtrl* shapeName;
	KaiTextCtrl* shapeAsASS;
	std::vector<ShapesSetting> shapes;
	ShapesSetting currentShape;
	int selection = 0;
};

class Shapes {
public:
	Shapes(DrawingAndClip* vis) : visual(vis) { 
		shapethis = this;
	};
	~Shapes() { delete shapes; };
	static std::vector<ShapesSetting>* GetShapes() { return shapethis->shapes; };
	static void SetShapes(std::vector<ShapesSetting>* shapes) { 
		if (shapethis->shapes) {
			delete shapethis->shapes;
			shapethis->shapes = NULL;
		}
		shapethis->shapes = new std::vector<ShapesSetting>(*shapes); 
	}
	void OnMouseEvent(wxMouseEvent& evt);
	void OnPaint();
	void SetShape(int shape);
	void GetVisual(wxString* drawing);
	void SetScale(wxString* txt, size_t position);
private:
	void SortPoints();
	void SetDrawingScale();
	int HitTest(const D3DXVECTOR2& pos, bool diff = false);
	DrawingAndClip* visual = NULL;
	std::vector<ShapesSetting> *shapes = NULL;
	int shape = 0;
	ShapesSetting currentShape;
	std::vector<ClipPoint> points;
	static Shapes* shapethis;
	D3DXVECTOR2 shapeSize;
	D3DXVECTOR2 scale = D3DXVECTOR2(1.f, 1.f);
	D3DXVECTOR2 drawingRectangle[2] = { D3DXVECTOR2(0.f, 0.f), D3DXVECTOR2(0.f, 0.f) };
	bool rectangleVisible = false;
	int grabbed = -1;
};

void LoadSettings(std::vector<ShapesSetting>* shapes);

void GetNames(std::vector<ShapesSetting>* shapes, wxArrayString* nameList);

void SaveSettings(std::vector<ShapesSetting>* shapes);
