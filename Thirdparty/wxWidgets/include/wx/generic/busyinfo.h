/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/busyinfo.h
// Purpose:     Information window (when app is busy)
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUSYINFO_H_
#define _WX_BUSYINFO_H_

#include "wx/defs.h"

#if wxUSE_BUSYINFO

#include "wx/object.h"

class  wxFrame;
class  wxWindow;

//--------------------------------------------------------------------------------
// wxBusyInfo
//                  Displays progress information
//                  Can be used in exactly same way as wxBusyCursor
//--------------------------------------------------------------------------------

class  wxBusyInfo : public wxObject
{
public:
    wxBusyInfo(const wxString& message, wxWindow *parent = NULL);

    virtual ~wxBusyInfo();

private:
    wxFrame *m_InfoFrame;

    wxDECLARE_NO_COPY_CLASS(wxBusyInfo);
};

#endif // wxUSE_BUSYINFO
#endif // _WX_BUSYINFO_H_
