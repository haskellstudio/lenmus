//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
//
//    This file is derived from file src/generic/mdig.h from wxWidgets 2.7.1 project.
//    Author:       Hans Van Leemputten
//    Copyright (c) Hans Van Leemputten
// 
//    Modified by:
//        Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the 
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 2 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this 
//    program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, 
//    Fifth Floor, Boston, MA  02110-1301, USA.
//
//    For any comment, suggestion or feature request, please contact the manager of 
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LM_NOTEBOOKMDI_H__
#define __LM_NOTEBOOKMDI_H__

#include "../app/global.h"

#if lmUSE_NOTEBOOK_MDI

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/frame.h"
#include "wx/panel.h"
#include "wx/notebook.h"

extern WXDLLEXPORT_DATA(const wxChar) wxStatusLineNameStr[];


//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class lmMDIParentFrame;
class lmMDIClientWindow;
class lmMDIChildFrame;

//-----------------------------------------------------------------------------
// lmMDIParentFrame
//-----------------------------------------------------------------------------

class lmMDIParentFrame: public wxFrame
{
public:
    lmMDIParentFrame();
    lmMDIParentFrame(wxWindow *parent,
                     wxWindowID winid,
                     const wxString& title,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
                     const wxString& name = wxFrameNameStr);

    virtual ~lmMDIParentFrame();
    bool Create( wxWindow *parent,
                 wxWindowID winid,
                 const wxString& title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
                 const wxString& name = wxFrameNameStr );


    virtual void SetMenuBar(wxMenuBar *pMenuBar);
    virtual bool ProcessEvent(wxEvent& event);

    lmMDIChildFrame *GetActiveChild() const;
    inline void SetActiveChild(lmMDIChildFrame* pChildFrame);

    lmMDIClientWindow *GetClientWindow() const;

    virtual void ActivateNext();
    virtual void ActivatePrevious();
    virtual void CloseAll();
    virtual void CloseActive();

    void RemoveChildFrame(lmMDIChildFrame* pChild);
    void OnSize(wxSizeEvent& event);

protected:
    void Init();
    virtual void DoGetClientSize(int *width, int *height) const;


    lmMDIClientWindow   *m_pClientWindow;
    lmMDIChildFrame     *m_pActiveChild;
    wxMenuBar           *m_pMyMenuBar;

private:
    DECLARE_DYNAMIC_CLASS(lmMDIParentFrame)
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// lmMDIChildFrame
// It is a panel on the Notebook (MDIParentFrame)
//-----------------------------------------------------------------------------

class lmMDIChildFrame: public wxPanel
{
public:
    lmMDIChildFrame();
    lmMDIChildFrame( lmMDIParentFrame *parent,
                     wxWindowID winid,
                     const wxString& title,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_FRAME_STYLE,
                     const wxString& name = wxFrameNameStr );

    virtual ~lmMDIChildFrame();
    bool Create( lmMDIParentFrame *parent,
                 wxWindowID winid,
                 const wxString& title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE,
                 const wxString& name = wxFrameNameStr );

    virtual void SetTitle(const wxString& title);
    virtual wxString GetTitle() const;

    virtual void Activate();


    // no icon
    void SetIcon( const wxIcon& WXUNUSED(icon) ) { }
    void SetIcons( const wxIconBundle& WXUNUSED(icons) ) { }

    // no maximize etc
    virtual void Maximize( bool WXUNUSED(maximize) = true) { /* Has no effect */ }
    virtual void Restore() { /* Has no effect */ }
    virtual void Iconize(bool WXUNUSED(iconize)  = true) { /* Has no effect */ }
    virtual bool IsMaximized() const { return true; }
    virtual bool IsIconized() const { return false; }
    virtual bool ShowFullScreen(bool WXUNUSED(show), long WXUNUSED(style)) { return false; }
    virtual bool IsFullScreen() const { return false; }

    virtual bool IsTopLevel() const { return false; }


    // The next 2 are copied from top level...
    void OnCloseWindow(wxCloseEvent& event);
    void OnSize(wxSizeEvent& event);

    void SetMDIParentFrame(lmMDIParentFrame* parentFrame);
    lmMDIParentFrame* GetMDIParentFrame() const;

protected:
    lmMDIParentFrame *m_pMDIParentFrame;
    wxRect            m_MDIRect;
    wxString          m_Title;


protected:
    void Init();

    virtual void DoMoveWindow(int x, int y, int width, int height);

    // This function needs to be called when a size change is confirmed,
    // we needed this function to prevent any body from the outside
    // changing the panel... it messes the UI layout when we would allow it.
    void ApplyMDIChildFrameRect();

private:
    DECLARE_DYNAMIC_CLASS(lmMDIChildFrame)
    DECLARE_EVENT_TABLE()

    friend class lmMDIClientWindow;
};

//-----------------------------------------------------------------------------
// lmMDIClientWindow
//-----------------------------------------------------------------------------

class lmMDIClientWindow: public wxNotebook
{
public:
    lmMDIClientWindow();
    lmMDIClientWindow( lmMDIParentFrame *parent, long style = 0 );
    virtual ~lmMDIClientWindow();

    virtual int SetSelection(size_t nPage);
    void OnSize(wxSizeEvent& event);

protected:
    void PageChanged(int OldSelection, int newSelection);

    void OnPageChanged(wxNotebookEvent& event);

private:
    DECLARE_DYNAMIC_CLASS(lmMDIClientWindow)
    DECLARE_EVENT_TABLE()
};


#else   // do not lmUSE_NOTEBOOK_MDI

//use standard wxWidgets classes
#define lmMDIParentFrame wxMDIParentFrame
#define lmMDIClientWindow wxMDIClientWindow
#define lmMDIChildFrame wxMDIChildFrame


#endif  //lmUSE_NOTEBOOK_MDI


#endif      // __LM_NOTEBOOKMDI_H__
