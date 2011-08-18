//---------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2011 LenMus project
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//---------------------------------------------------------------------------------------

//#ifndef __LENMUS_EARCOMPAREINTVCTROL_H__        //to avoid nested includes
//#define __LENMUS_EARCOMPAREINTVCTROL_H__
//
//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma interface "EarCompareIntvCtrol.cpp"
//#endif
//
//// For compilers that support precompilation, includes <wx/wx.h>.
//#include <wx/wxprec.h>
//
//#ifdef __BORLANDC__
//#pragma hdrstop
//#endif
//
//#ifndef WX_PRECOMP
//#include <wx/wx.h>
//#endif
//
//#include "EarIntvalConstrains.h"
//#include "../score/Score.h"
//#include "ExerciseCtrol.h"
//
//
//class lmEarCompareIntvCtrol : public lmCompareScoresCtrol
//{
//   DECLARE_DYNAMIC_CLASS(lmEarCompareIntvCtrol)
//
//public:
//
//    // constructor and destructor
//    lmEarCompareIntvCtrol(wxWindow* parent, wxWindowID id,
//               lmEarIntervalsConstrains* pConstrains,
//               const wxPoint& pos = wxDefaultPosition,
//               const wxSize& size = wxDefaultSize, int style = 0);
//
//    ~lmEarCompareIntvCtrol();
//
//    // implementation of virtual methods
//    wxString SetNewProblem();
//    wxDialog* GetSettingsDlg();
//    void PrepareAuxScore(int nButton) {}
//
//
//private:
//
//        // member variables
//
//    lmEarIntervalsConstrains* m_pConstrains;    //use same constraints than for intervals
//    lmMPitch        m_ntMidi[2];            //the midi pitch of the two notes
//    lmMPitch        m_ntPitch[2];           //the pitch of the two notes
//
//};
//
//
//
//#endif  // __LENMUS_EARCOMPAREINTVCTROL_H__