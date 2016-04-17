#ifndef VIDEOPROGBAR_H_INCLUDED
#define VIDEOPROGBAR_H_INCLUDED

#include <wx/panel.h>
#include <wx/font.h>
#include <wx/wx.h>
#include "VideoSlider.h"
#include "BitmapButton.h"

class Fullscreen : public wxFrame
{
    public:
    Fullscreen(wxWindow* parent, const wxPoint& pos, const wxSize &size);
    virtual ~Fullscreen();
	BitmapButton* bprev;
	BitmapButton* bpause;
	BitmapButton* bstop;
	BitmapButton* bnext;
	BitmapButton* bpline;
	wxStaticText* Videolabel;
	VideoSlider* vslider;
	VolSlider* volslider;
	wxPanel* panel;
    

    private:
    wxWindow *vb;
};


#endif // VIDEOPROGBAR_H_INCLUDED
