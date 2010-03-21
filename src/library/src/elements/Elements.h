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

#ifndef __LM_ELEMENTS_H__        //to avoid nested includes
#define __LM_ELEMENTS_H__

#include <vector>
#include "../base/SmartPointer.h"
#include "../base/SimpleTree.h"
#include "../visitors/Visitor.h"
#include "../global/StringType.h"


namespace lenmus
{

enum ELdpElements
{
    eElmFirst = 0,
    k_undefined = eElmFirst,
    
    //simple, generic elements
    k_label,
    k_number,
    k_string,

    //composite elements
    k_abbrev,
    k_above,
    k_barline,
    k_below,
    k_bold,
    k_bold_italic,
    k_brace,
    k_bracket,
    k_center,
    k_chord,
    k_clef,
    k_color,        
    k_defineStyle,
    k_down,
    k_duration,
    k_dx,
    k_dy,
    k_end,
    k_fermata,
    k_font,
    k_goBack,
    k_goFwd,
    k_graphic,
    k_group,
    k_hasWidth,
    k_infoMIDI,
    k_instrName,
    k_instrument,
    k_italic,
    k_joinBarlines,
    k_key,
    k_landscape,
    k_language,
    k_left,
    k_metronome,
    k_musicData,
    k_name,
    k_newSystem,
    k_no,
    k_normal,
    k_noVisible,
    k_opt,
    k_pageLayout,
    k_pageMargins,
    k_pageSize,
    k_parentheses,
    k_pitch,
    k_portrait,
    k_right,
    k_score,
    k_spacer,
    k_split,
    k_start,
    k_staves,
    k_stem,
    k_style,
    k_symbol,
    k_systemLayout,
    k_systemMargins,
    k_text,
    k_time,
    k_title,
    k_vers,
    k_up,
    k_yes,

    //single char elements
    k_g,   //beamed group 
    k_l,   //tie 
    k_note,   //note
    k_p,   //staff number  
    k_r,   //rest 
    k_t,   //tuplet 
    k_v,   //voice

    k_line,

    eElmLast,
};

class LdpElement;
typedef SmartPtr<LdpElement>    SpLdpElement;

/*!
 \brief A generic LDP element representation.

	An element is a node in the score tree, combining links to other nodes as well as
    the actual element data.
    There are two types of elements:
    - simple: it is just a type (label, string or number) and its value (they are 
        similar to LISP atoms)
    - composite: they have a name and any number of parameters (zero allowed)n (they 
        are like LISP lists)
*/
class LM_EXPORT LdpElement : public Visitable, virtual public RefCounted,
                             public SimpleTree<LdpElement>
{
protected:
	int	m_type;             ///< the element type
	string_type	m_name;     ///< element name, for composite elements
	string_type m_value;    ///< the element value, for simple elements
    bool m_fSimple;         ///< true for simple elements

    LdpElement();
	virtual ~LdpElement() {}

public:
 	//typedef SimpleTree<LdpElement>::iterator			iterator;

	static SmartPtr<LdpElement> create();

    //overrides to Visitable class members
	virtual void accept_in(BaseVisitor& v);
	virtual void accept_out(BaseVisitor& v);

    //getters and setters
	inline void set_value(const string_type& value) { m_value = value; }
    inline void set_name(const string_type& name) { m_name = name; }

	inline int get_type() { return m_type; }
	inline const string_type& get_name() { return m_name; }

	//! returns the element value as it is represented in source LDP
	string_type get_ldp_value();
	//! elements comparison
	bool operator ==(LdpElement& element);
	inline bool operator !=(LdpElement& element) { return !(*this == element); }

    //Compatibility with lmLDPNode
	//void DumpNode(wxString sIndent=_T("");
    string_type to_string();

    inline bool is_simple() { return m_fSimple; }
    inline void set_simple() { m_fSimple = true; }
	inline bool has_children() { return !elements().empty(); }
    //inline bool IsProcessed() { return m_fProcessed; }
    //inline void SetProcessed(bool fValue) { m_fProcessed = fValue; }

    //inline long GetNumLine() { return m_nNumLine; }
    //inline long GetID() { return m_nID; }
    int get_num_parameters();

    //! random access to parameter i (0..n-1)
    LdpElement* get_parameter(int i);
    //lmLDPNode* get_parameter(wxString& sName);

    ////iteration
    //lmLDPNode* StartIterator(long iP=1, bool fOnlyNotProcessed = true);
    //lmLDPNode* GetNextParameter(bool fOnlyNotProcessed = true);

//private:
//    wxString        m_sName;            //node name
//    long            m_nID;              //element ID
//    long            m_nNumLine;         //LDP source file: line number
//    bool            m_fIsSimple;        //the node is simple (just a string)
//    bool            m_fProcessed;       //the node has been processed
//	std::vector<lmLDPNode*> m_cNodes;	//Parameters of this node
//    std::vector<lmLDPNode*>::iterator   m_it;       //for sequential accsess
};

/*!
 \brief A generic LDP element representation.

    For each ldp element we define a specific class. It is equivalent to defining
    specific lmLDPNodes for each ldp tag. In this way we have specific nodes
    LdpObject<type>.
*/
template <int type>
class LdpObject : public LdpElement
{ 
    protected:	
        LdpObject() : LdpElement() { m_type = type; }

	public:
        //! static constructor to be used by Factory
		static SmartPtr<LdpObject<type> > NewLdpObject()	
			{ LdpObject<type>* o = new LdpObject<type>; assert(o!=0); return o; }

        //! implementation of Visitable interface
        virtual void accept_in(BaseVisitor& v) {
			if (Visitor<SmartPtr<LdpObject<type> > >* p = dynamic_cast<Visitor<SmartPtr<LdpObject<type> > >*>(&v))
            {
				SmartPtr<LdpObject<type> > sptr = this;
				p->VisitStart(sptr);
			}
			else LdpElement::accept_in(v);
		}

        virtual void accept_out(BaseVisitor& v) {
			if (Visitor<SmartPtr<LdpObject<type> > >* p = dynamic_cast<Visitor<SmartPtr<LdpObject<type> > >*>(&v))
            {
				SmartPtr<LdpObject<type> > sptr = this;
				p->VisitEnd(sptr);
			}
			else LdpElement::accept_out(v);
		}
};

//typedefs for all LDP elements
typedef SmartPtr<LdpObject<k_abbrev> >       SpLdpAbbrev;
typedef SmartPtr<LdpObject<k_above> >        SpLdpAbove;
typedef SmartPtr<LdpObject<k_barline> >      SpLdpBarline;
typedef SmartPtr<LdpObject<k_below> >        SpLdpBelow;
typedef SmartPtr<LdpObject<k_bold> >         SpLdpBold;
typedef SmartPtr<LdpObject<k_bold_italic> >  SpLdpBoldItalic;
typedef SmartPtr<LdpObject<k_brace> >        SpLdpBrace;
typedef SmartPtr<LdpObject<k_bracket> >      SpLdpBracket;
typedef SmartPtr<LdpObject<k_center> >       SpLdpCenter;
typedef SmartPtr<LdpObject<k_chord> >        SpLdpChord;
typedef SmartPtr<LdpObject<k_clef> >         SpLdpClef;
typedef SmartPtr<LdpObject<k_color> >        SpLdpColor;            
typedef SmartPtr<LdpObject<k_defineStyle> >  SpLdpDefineStyle;
typedef SmartPtr<LdpObject<k_down> >         SpLdpDown;
typedef SmartPtr<LdpObject<k_duration> >     SpLdpDuration;
typedef SmartPtr<LdpObject<k_dx> >           SpLdpDx;
typedef SmartPtr<LdpObject<k_dy> >           SpLdpDy;
typedef SmartPtr<LdpObject<k_end> >          SpLdpEnd;
typedef SmartPtr<LdpObject<k_fermata> >      SpLdpFermata;
typedef SmartPtr<LdpObject<k_font> >         SpLdpFont;
typedef SmartPtr<LdpObject<k_goBack> >       SpLdpGoBack;
typedef SmartPtr<LdpObject<k_goFwd> >        SpLdpGoFwd;
typedef SmartPtr<LdpObject<k_graphic> >      SpLdpGraphic;
typedef SmartPtr<LdpObject<k_group> >        SpLdpGroup;
typedef SmartPtr<LdpObject<k_hasWidth> >     SpLdpHasWidth;
typedef SmartPtr<LdpObject<k_infoMIDI> >     SpLdpInfoMIDI;
typedef SmartPtr<LdpObject<k_instrName> >    SpLdpInstrName;
typedef SmartPtr<LdpObject<k_instrument> >   SpLdpInstrument;
typedef SmartPtr<LdpObject<k_italic> >       SpLdpItalic;
typedef SmartPtr<LdpObject<k_joinBarlines> > SpLdpJoinBarlines;
typedef SmartPtr<LdpObject<k_key> >          SpLdpKey;
typedef SmartPtr<LdpObject<k_landscape> >    SpLdpLandscape;
typedef SmartPtr<LdpObject<k_left> >         SpLdpLeft;
typedef SmartPtr<LdpObject<k_line> >         SpLdpLine;
typedef SmartPtr<LdpObject<k_metronome> >    SpLdpMetronome;
typedef SmartPtr<LdpObject<k_musicData> >    SpLdpMusicData;
typedef SmartPtr<LdpObject<k_name> >         SpLdpName;
typedef SmartPtr<LdpObject<k_newSystem> >    SpLdpNewSystem;
typedef SmartPtr<LdpObject<k_no> >           SpLdpNo;
typedef SmartPtr<LdpObject<k_normal> >       SpLdpNormal;
typedef SmartPtr<LdpObject<k_noVisible> >    SpLdpNoVisible;
typedef SmartPtr<LdpObject<k_opt> >          SpLdpOpt;
typedef SmartPtr<LdpObject<k_pageLayout> >   SpLdpPageLayout;
typedef SmartPtr<LdpObject<k_pageMargins> >  SpLdpPageMargins;
typedef SmartPtr<LdpObject<k_pageSize> >     SpLdpPageSize;
typedef SmartPtr<LdpObject<k_parentheses> >  SpLdpParentheses;
typedef SmartPtr<LdpObject<k_pitch> >        SpLdpPitch;
typedef SmartPtr<LdpObject<k_portrait> >     SpLdpPortrait;
typedef SmartPtr<LdpObject<k_right> >        SpLdpRight;
typedef SmartPtr<LdpObject<k_score> >        SpLdpScore;
typedef SmartPtr<LdpObject<k_spacer> >       SpLdpSpacer;
typedef SmartPtr<LdpObject<k_split> >        SpLdpSplit;
typedef SmartPtr<LdpObject<k_start> >        SpLdpStart;
typedef SmartPtr<LdpObject<k_staves> >       SpLdpStaves;
typedef SmartPtr<LdpObject<k_stem> >         SpLdpStem;
typedef SmartPtr<LdpObject<k_style> >        SpLdpStyle;
typedef SmartPtr<LdpObject<k_string> >       SpLdpString;
typedef SmartPtr<LdpObject<k_symbol> >       SpLdpSymbol;
typedef SmartPtr<LdpObject<k_text> >         SpLdpText;
typedef SmartPtr<LdpObject<k_time> >         SpLdpTime;
typedef SmartPtr<LdpObject<k_title> >        SpLdpTitle;
typedef SmartPtr<LdpObject<k_up> >           SpLdpUp;
typedef SmartPtr<LdpObject<k_yes> >          SpLdpYes;

typedef SmartPtr<LdpObject<k_g> >            SpLdpG;   //beamed group 
typedef SmartPtr<LdpObject<k_l> >            SpLdpL;   //tie 
typedef SmartPtr<LdpObject<k_note> >         SpLdpNote;   // "n"
typedef SmartPtr<LdpObject<k_p> >            SpLdpP;   //staff number  
typedef SmartPtr<LdpObject<k_r> >            SpLdpR;   //rest 
typedef SmartPtr<LdpObject<k_t> >            SpLdpT;   //tuplet 
typedef SmartPtr<LdpObject<k_v> >            SpLdpV;   //voice


}   //namespace lenmus
#endif    // __LM_ELEMENTS_H__

