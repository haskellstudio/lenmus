//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2006 Cecilio Salmeron
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
/*! @file LDPTags.cpp
    @brief Implementation file for class lmLDPTags
    @ingroup ldp_parser
*/
/*! @class lmLDPTags
    @ingroup ldp_parser
    @brief lmLDPTags This class is a singleton to contain the LDP tags table.

	Tag translations are going to be stored in a file named 'LDP_Tags.txt' in
	current language directory.
	On object initialization the Spanish tags set will be loaded.

	When parsing a score, if the score has a 'lang=xx' tag, then the tags set
	for that language will be loaded.

	The 'score' and 'language' tags are the only non-translatable, according to
	LDP standard v4.0

*/


#ifdef __GNUG__
// #pragma implementation
#endif

// for (compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "LDPTags.h"

// ----------------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------------

//initialize the only instance
lmLdpTagsTable* lmLdpTagsTable::m_pInstance = (lmLdpTagsTable*)NULL;

//destructor
lmLdpTagsTable::~lmLdpTagsTable()
{
}

lmLdpTagsTable* lmLdpTagsTable::GetInstance()
{
	if (!m_pInstance) {
		m_pInstance = new lmLdpTagsTable();
	}
	return m_pInstance;
}

void lmLdpTagsTable::DeleteInstance()
{
	if (m_pInstance) {
		delete m_pInstance;
	    m_pInstance = (lmLdpTagsTable*)NULL;
	}
}

const wxString& lmLdpTagsTable::TagName(wxString sTag, wxString sContext)
{
    if (sContext == _T(""))
        return m_Tags[0][sTag];
    else {
        int iT = m_Contexts[sContext];
        wxASSERT( iT <= lmMAX_TAG_CONTEXTS );
        return m_Tags[iT][sTag];
    }
}

//load tags set for given language
void lmLdpTagsTable::LoadTags(wxString sLanguage, wxString sCharset)
{
    //! @todo For now charset is ignored

    //clear tables
    m_Contexts.clear();
    int i;
    for (i=0; i <= lmMAX_TAG_CONTEXTS; i++) {
        m_Tags[i].clear();
    }


    //! @todo   For now I will load tags from program. This must be changed to load them
    //!         from file ''LDP_Tags.txt' in given language directory.

    //! @todo   For now I will load Spanish tags as default. This must not be changed
    //!         while program generated tags are in Spanish.
    if (sLanguage == _T("en")) {
        m_Tags[0][_T("abbrev")] = _T("abbrev");
        m_Tags[0][_T("barline")] = _T("barline");
        m_Tags[0][_T("bold")] = _T("bold");
        m_Tags[0][_T("bold_italic")] = _T("bold-italic");
        m_Tags[0][_T("center")] = _T("center");
        m_Tags[0][_T("chord")] = _T("chord");
        m_Tags[0][_T("clef")] = _T("clef");
        m_Tags[0][_T("down")] = _T("down");
        m_Tags[0][_T("dx")] = _T("dx");
        m_Tags[0][_T("dy")] = _T("dy");
        m_Tags[0][_T("font")] = _T("font");
        m_Tags[0][_T("hasWidth")] = _T("hasWidth");
        m_Tags[0][_T("infoMIDI")] = _T("infoMIDI");
        m_Tags[0][_T("instrName")] = _T("instrName");
        m_Tags[0][_T("instrument")] = _T("instrument");
        m_Tags[0][_T("italic")] = _T("italic");
        m_Tags[0][_T("key")] = _T("key");
        m_Tags[0][_T("left")] = _T("left");
        m_Tags[0][_T("name")] = _T("name");
        m_Tags[0][_T("newSystem")] = _T("newSystem");
        m_Tags[0][_T("normal")] = _T("normal");
        m_Tags[0][_T("right")] = _T("right");
        m_Tags[0][_T("split")] = _T("split");
        m_Tags[0][_T("staves")] = _T("staves");
        m_Tags[0][_T("stem")] = _T("stem");
        m_Tags[0][_T("text")] = _T("text");
        m_Tags[0][_T("time")] = _T("time");
        m_Tags[0][_T("title")] = _T("title");
        m_Tags[0][_T("up")] = _T("up");
        m_Tags[0][_T("voice")] = _T("voice");
        m_Tags[0][_T("x")] = _T("x");
        m_Tags[0][_T("y")] = _T("y");

        //special contexts
        m_Contexts[_T("NoteType")] = 1;
        m_Tags[1][_T("d")] = _T("d");
        m_Tags[1][_T("r")] = _T("w");
        m_Tags[1][_T("b")] = _T("h");
        m_Tags[1][_T("n")] = _T("q");
        m_Tags[1][_T("c")] = _T("e");
        m_Tags[1][_T("s")] = _T("s");
        m_Tags[1][_T("f")] = _T("t");
        m_Tags[1][_T("m")] = _T("x");
        m_Tags[1][_T("g")] = _T("o");
        m_Tags[1][_T("p")] = _T("f");

        m_Contexts[_T("Barlines")] = 2;
        m_Tags[2][_T("simple")] = _T("simple");
        m_Tags[2][_T("double")] = _T("double");
        m_Tags[2][_T("end")] = _T("end");
        m_Tags[2][_T("start")] = _T("start");
        m_Tags[2][_T("startRepetition")] = _T("startRepetition");
        m_Tags[2][_T("endRepetition")] = _T("endRepetition");
        m_Tags[2][_T("doubleRepetition")] = _T("doubleRepetition");

    }
    else {
        // initialize table with default Spanish values

        //no context
        m_Tags[0][_T("abbrev")] = _T("abrev");
        m_Tags[0][_T("barline")] = _T("Barra");            //! @todo change for 1.4
        m_Tags[0][_T("bold")] = _T("negrita");
        m_Tags[0][_T("bold_italic")] = _T("negrita-cursiva");
        m_Tags[0][_T("center")] = _T("centrado");
        m_Tags[0][_T("chord")] = _T("acorde");
        m_Tags[0][_T("clef")] = _T("Clave");            //! @todo change for 1.4
        m_Tags[0][_T("down")] = _T("abajo");
        m_Tags[0][_T("dx")] = _T("dx");
        m_Tags[0][_T("dy")] = _T("dy");
        m_Tags[0][_T("font")] = _T("font");
        m_Tags[0][_T("hasWidth")] = _T("tieneAnchura");
        m_Tags[0][_T("infoMIDI")] = _T("infoMIDI");
        m_Tags[0][_T("instrName")] = _T("nombreInstrumento");
        m_Tags[0][_T("instrument")] = _T("instrumento");
        m_Tags[0][_T("italic")] = _T("cursiva");
        m_Tags[0][_T("key")] = _T("Tonalidad");          //! @todo change for 1.4
        m_Tags[0][_T("left")] = _T("izqda");
        m_Tags[0][_T("name")] = _T("nombre");
        m_Tags[0][_T("newSystem")] = _T("nuevoSistema");
        m_Tags[0][_T("normal")] = _T("normal");
        m_Tags[0][_T("right")] = _T("dcha");
        m_Tags[0][_T("split")] = _T("partes");
        m_Tags[0][_T("staves")] = _T("numPentagramas");
        m_Tags[0][_T("stem")] = _T("plica");
        m_Tags[0][_T("text")] = _T("texto");
        m_Tags[0][_T("time")] = _T("Metrica");        //! @todo change for 1.4
        m_Tags[0][_T("title")] = _T("titulo");
        m_Tags[0][_T("up")] = _T("arriba");
        m_Tags[0][_T("voice")] = _T("voz");
        m_Tags[0][_T("x")] = _T("x");
        m_Tags[0][_T("y")] = _T("y");

        //special contexts
        m_Contexts[_T("NoteType")] = 1;
        m_Tags[1][_T("d")] = _T("d");
        m_Tags[1][_T("r")] = _T("r");
        m_Tags[1][_T("b")] = _T("b");
        m_Tags[1][_T("n")] = _T("n");
        m_Tags[1][_T("c")] = _T("c");
        m_Tags[1][_T("s")] = _T("s");
        m_Tags[1][_T("f")] = _T("f");
        m_Tags[1][_T("m")] = _T("m");
        m_Tags[1][_T("g")] = _T("g");
        m_Tags[1][_T("p")] = _T("p");

        m_Contexts[_T("Barlines")] = 2;
        m_Tags[2][_T("simple")] = _T("Simple");
        m_Tags[2][_T("double")] = _T("Doble");
        m_Tags[2][_T("end")] = _T("Final");
        m_Tags[2][_T("start")] = _T("Inicial");
        m_Tags[2][_T("startRepetition")] = _T("InicioRepeticion");
        m_Tags[2][_T("endRepetition")] = _T("FinRepeticion");
        m_Tags[2][_T("doubleRepetition")] = _T("DobleRepeticion");
    }

}

