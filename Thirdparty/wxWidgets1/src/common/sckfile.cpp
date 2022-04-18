/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/sckfile.cpp
// Purpose:     File protocol
// Author:      Guilhem Lavaux
// Modified by:
// Created:     20/07/97
// RCS-ID:      $Id$
// Copyright:   (c) 1997, 1998 Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_STREAMS && wxUSE_PROTOCOL_FILE

#ifndef WX_PRECOMP
#endif

#include "wx/uri.h"
#include "wx/wfstream.h"
#include "wx/protocol/file.h"


// ----------------------------------------------------------------------------
// wxFileProto
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFileProto, wxProtocol)
IMPLEMENT_PROTOCOL(wxFileProto, wxT("file"), nullptr, false)

wxFileProto::wxFileProto()
           : wxProtocol()
{
}

wxFileProto::~wxFileProto()
{
}

wxInputStream *wxFileProto::GetInputStream(const wxString& path)
{
    wxFileInputStream *retval = new wxFileInputStream(wxURI::Unescape(path));
    if ( retval->IsOk() )
    {
        m_lastError = wxPROTO_NOERR;
        return retval;
    }

    m_lastError = wxPROTO_NOFILE;
    delete retval;

    return nullptr;
}

#endif // wxUSE_STREAMS && wxUSE_PROTOCOL_FILE