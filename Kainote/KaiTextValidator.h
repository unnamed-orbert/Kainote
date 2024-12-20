/////////////////////////////////////////////////////////////////////////////
// Name:        wx/valtext.h
// Purpose:     KaiTextValidator class
// Author:      Julian Smart
// Modified by: Francesco Montorsi
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once



class KaiTextCtrl;
#include <wx/msw/winundef.h>
#include "wx/defs.h"
#include "wx/validate.h"
#include "wx/valtext.h"


class KaiTextValidator: public wxValidator
{
public:
    KaiTextValidator(long style = wxFILTER_NONE, wxString *val = nullptr);
    KaiTextValidator(const KaiTextValidator& val);

    virtual ~KaiTextValidator(){}

    // Make a clone of this validator (or return nullptr) - currently necessary
    // if you're passing a reference to a validator.
    // Another possibility is to always pass a pointer to a new validator
    // (so the calling code can use a copy constructor of the relevant class).
    virtual wxObject *Clone() const { return new KaiTextValidator(*this); }
    bool Copy(const KaiTextValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);

    // Called to transfer data to the window
    virtual bool TransferToWindow();

    // Called to transfer data from the window
    virtual bool TransferFromWindow();

    // Filter keystrokes
    void OnChar(wxKeyEvent& event);

    // ACCESSORS
    inline long GetStyle() const { return m_validatorStyle; }
    void SetStyle(long style);

    KaiTextCtrl *GetKaiTextCtrl();

    void SetCharIncludes(const wxString& chars);
    void SetIncludes(const wxArrayString& includes) { m_includes = includes; }
    inline wxArrayString& GetIncludes() { return m_includes; }

    void SetCharExcludes(const wxString& chars);
    void SetExcludes(const wxArrayString& excludes) { m_excludes = excludes; }
    inline wxArrayString& GetExcludes() { return m_excludes; }

    bool HasFlag(wxTextValidatorStyle style) const
        { return (m_validatorStyle & style) != 0; }

protected:

    // returns true if all characters of the given string are present in m_includes
    bool ContainsOnlyIncludedCharacters(const wxString& val) const;

    // returns true if at least one character of the given string is present in m_excludes
    bool ContainsExcludedCharacters(const wxString& val) const;

    // returns the error message if the contents of 'val' are invalid
    virtual wxString IsValid(const wxString& val) const;

protected:
    long                 m_validatorStyle;
    wxString*            m_stringValue;
    wxArrayString        m_includes;
    wxArrayString        m_excludes;

private:
    wxDECLARE_NO_ASSIGN_CLASS(KaiTextValidator);
    DECLARE_DYNAMIC_CLASS(KaiTextValidator)
    DECLARE_EVENT_TABLE()
};


