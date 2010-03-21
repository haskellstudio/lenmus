//--------------------------------------------------------------------------------------
//  LenMus Library
//  Copyright (c) 2010 LenMus project
//
//  This program is free software; you can redistribute it and/or modify it under the 
//  terms of the GNU General Public License as published by the Free Software Foundation,
//  either version 3 of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//  PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License along
//  with this library; if not, see <http://www.gnu.org/licenses/> or write to the
//  Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
//  MA  02111-1307,  USA.
//
//  For any comment, suggestion or feature request, please contact the manager of
//  the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LM_LDP_PARSER_H__ 
#define __LM_LDP_PARSER_H__

#include <vector>
#include <set>
#include "LdpTokenizer.h"
#include "../global/StringType.h"
#include "../elements/Elements.h"

using namespace std;

namespace lenmus
{

/*!
 \brief The LDP parser
*/
class LdpParser
{
public:
    LdpParser(tostream& reporter);
    ~LdpParser();

//    //setings and options
//    inline void SetIgnoreList(std::set<long>* pSet) { m_pIgnoreSet = pSet; }
//
    SpLdpElement parse_file(const string_type& filename, bool fErrorMsg = true);
    SpLdpElement parse_text(const string_type& sourceText);
//
//    //for xxxxxxxCtrolParms
//    lmScore* AnalyzeScore(SpLdpElement pNode);
//    bool ParenthesisMatch(const string_type& sSource);


protected:
    enum EParsingState
    {
        A0_WaitingForStartOfElement = 0,
        A1_WaitingForName,
        A2_WaitingForParameter,
        A3_ProcessingParameter,
        A4_Exit,
        A5_ExitError
    };

    SpLdpElement do_syntax_analysis(LdpReader& reader);

    void initialize();
    void PushNode(EParsingState nPopState);
    bool PopNode();
    void Do_WaitingForStartOfElement();
    void Do_WaitingForName();
    void Do_WaitingForParameter();
    void Do_ProcessingParameter();

    void report_error(EParsingState nState, LdpToken* pTk);
    void report_error(SpLdpElement pNode, const char_type* szFormat, ...);

//    long GetNodeID(SpLdpElement pNode);
//
    long                m_numLine;         //number of lines read
//    long                m_nMaxID;           //maximun ID found

    tostream&       m_reporter;
    LdpTokenizer*   m_pTokenizer;
    LdpToken*       m_pTk;              ///< current token
    EParsingState   m_state;            ///< current automata state
    std::stack<pair<EParsingState, SpLdpElement> >  m_stack;    ///< To save current automata state and node
    SpLdpElement    m_curNode;             //node in process

    // parsing control, options and error variables
//    bool            m_fDebugMode;
    int            m_numErrors;            ///< number of errors found during parsing
//    std::set<long>*         m_pIgnoreSet;   //set with elements to ignore
};

} //namespace lenmus

#endif    //__LM_LDP_PARSER_H__
