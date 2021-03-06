//---------------------------------------------------------------------------------------
// This file is part of the Lomse library.
// Lomse is copyrighted work (c) 2010-2016. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this
//      list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright notice, this
//      list of conditions and the following disclaimer in the documentation and/or
//      other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// For any comment, suggestion or feature request, please contact the manager of
// the project at cecilios@users.sourceforge.net
//---------------------------------------------------------------------------------------

#include "lomse_staffobjs_table.h"

#include <algorithm>
#include "lomse_internal_model.h"
#include "lomse_im_note.h"
#include "lomse_ldp_exporter.h"
#include "lomse_time.h"
#include "lomse_im_factory.h"

//specific for ScoreAlgorithms
#include "lomse_pitch.h"


#include <sstream>
using namespace std;

namespace lomse
{

//=======================================================================================
// ColStaffObjsEntry implementation
//=======================================================================================
string ColStaffObjsEntry::dump(bool fWithIds)
{
    stringstream s;
    s << m_instr << "\t" << m_staff << "\t" << m_measure << "\t" << time() << "\t"
      << m_line << "\t" << (fWithIds ? to_string_with_ids() : to_string()) << endl;
    return s.str();
}

//---------------------------------------------------------------------------------------
std::string ColStaffObjsEntry::to_string()
{
    return m_pImo->to_string();
}

//---------------------------------------------------------------------------------------
std::string ColStaffObjsEntry::to_string_with_ids()
{
    return m_pImo->to_string_with_ids();
}



//=======================================================================================
// ColStaffObjs implementation
//=======================================================================================
ColStaffObjs::ColStaffObjs()
    : m_numLines(0)
    , m_numEntries(0)
    , m_rMissingTime(0.0)
    , m_pFirst(NULL)
    , m_pLast(NULL)
{
}

//---------------------------------------------------------------------------------------
ColStaffObjs::~ColStaffObjs()
{
    ColStaffObjs::iterator it;
    for (it=begin(); it != end(); ++it)
        delete *it;
}

//---------------------------------------------------------------------------------------
void ColStaffObjs::add_entry(int measure, int instr, int voice, int staff,
                             ImoStaffObj* pImo)
{
    ColStaffObjsEntry* pEntry =
        LOMSE_NEW ColStaffObjsEntry(measure, instr, voice, staff, pImo);
    add_entry_to_list(pEntry);
    ++m_numEntries;
}

//---------------------------------------------------------------------------------------
string ColStaffObjs::dump(bool fWithIds)
{
    stringstream s;
    ColStaffObjs::iterator it;
    s << "Num.entries = " << num_entries() << endl;
    //    +.......+.......+.......+.......+.......+.......+
    s << "instr   staff   meas.   time    line    object" << endl;
    s << "----------------------------------------------------------------" << endl;
    for (it=begin(); it != end(); ++it)
    {
        s << (*it)->dump(fWithIds);
    }
    return s.str();
}

//---------------------------------------------------------------------------------------
void ColStaffObjs::add_entry_to_list(ColStaffObjsEntry* pEntry)
{
    if (!m_pFirst)
    {
        //first entry
        m_pFirst = pEntry;
        m_pLast = pEntry;
        pEntry->set_prev( NULL );
        pEntry->set_next( NULL );
        return;
    }

    //insert in list in order
    ColStaffObjsEntry* pCurrent = m_pLast;
    while (pCurrent != NULL)
    {
        if (is_lower_entry(pEntry, pCurrent))
            pCurrent = pCurrent->get_prev();
        else
        {
            //insert after pCurrent
            ColStaffObjsEntry* pNext = pCurrent->get_next();
            pEntry->set_prev( pCurrent );
            pEntry->set_next( pNext );
            pCurrent->set_next( pEntry );
            if (pNext == NULL)
                m_pLast = pEntry;
            else
                pNext->set_prev( pEntry );
            return;
        }
    }

    //it is the first one
    pEntry->set_prev( NULL );
    pEntry->set_next( m_pFirst );
    m_pFirst->set_prev( pEntry );
    m_pFirst = pEntry;
}

//---------------------------------------------------------------------------------------
bool ColStaffObjs::is_lower_entry(ColStaffObjsEntry* b, ColStaffObjsEntry* a)
{
    //auxiliary, for sort: by time, object type (barlines before objects in other lines),
    //line and staff.
    //AWARE:
    //   Current order is first pA, then pB
    //   return TRUE to move pB before pA, FALSE to keep in current order


    //swap if B has lower time than A
    if ( is_lower_time(b->time(), a->time()) )
        return true;

    //both have equal time
    else if ( is_equal_time(b->time(), a->time()) )
    {
        ImoStaffObj* pB = b->imo_object();
        ImoStaffObj* pA = a->imo_object();

        //barline must go before all other objects at same measure
        //TODO: not asking for measure but for line. Is this correct?
        if (pB->is_barline() && !pA->is_barline() && b->line() != a->line())
            return true;
        else if (pA->is_barline() && !pB->is_barline() && b->line() != a->line())
            return false;

////        //note/rest can not go before non-timed
////        else if (pA->is_note_rest() && pB->get_duration() == 0.0f)
////            return true;
////        else if (pB->is_note_rest() && pA->get_duration() == 0.0f)
////            return false;
////
////        //clef in other staff can not go after key or time signature
////        else if (pB->is_clef() && (pA->is_key_signature() || pA->is_time_signature())
////                 && b->staff() != a->staff())
////            return true;
////        else if (pA->is_clef() && (pB->is_key_signature() || pB->is_time_signature()))
////            return false;
//
//        //else order by staff and line
//        return (b->line() < a->line() || (b->line() == a->line()
//                                          && b->staff() < a->staff()) );
        //else, preserve definition order
        return false;
    }

    //time(pB) > time(pA). They are correctly ordered
    return false;
}

//---------------------------------------------------------------------------------------
void ColStaffObjs::delete_entry_for(ImoStaffObj* pSO)
{
    ColStaffObjsEntry* pEntry = find_entry_for(pSO);
    if (!pEntry)
    {
        LOMSE_LOG_ERROR("[ColStaffObjs::delete_entry_for] entry not found!");
        throw runtime_error("[ColStaffObjs::delete_entry_for] entry not found!");
    }

    ColStaffObjsEntry* pPrev = pEntry->get_prev();
    ColStaffObjsEntry* pNext = pEntry->get_next();
    delete pEntry;
    if (pPrev == NULL)
    {
        //removing the head of the list
        m_pFirst = pNext;
        if (pNext)
            pNext->set_prev(NULL);
    }
    else if (pNext == NULL)
    {
        //removing the tail of the list
        m_pLast = pPrev;
        pPrev->set_next(NULL);
    }
    else
    {
        //removing intermediate node
        pPrev->set_next( pNext );
        pNext->set_prev( pPrev );
    }
    --m_numEntries;
}

//---------------------------------------------------------------------------------------
ColStaffObjsEntry* ColStaffObjs::find_entry_for(ImoStaffObj* pSO)
{
    ColStaffObjs::iterator it;
    for (it=begin(); it != end(); ++it)
    {
        if ((*it)->imo_object() == pSO)
            return *it;
    }
    return NULL;
}

//---------------------------------------------------------------------------------------
void ColStaffObjs::sort_table()
{
    //The table is created with entries in order. But edition operations forces to
    //reorder entries. The main requirement for the sort algorithm is:
    // * stable (preserve order of elements with equal keys)
    //The chosen algorithm is the insertion sort, that also has some advantages:
    // * good performance when table is nearly ordered
    // * simple to implement and much better performance than other simple algorithms,
    //   such as the bubble sort
    // * in-place sort (does not require extra memory)

    ColStaffObjsEntry* pUnsorted = m_pFirst;
    m_pFirst = NULL;
    m_pLast = NULL;

    while (pUnsorted != NULL)
    {
        ColStaffObjsEntry* pCurrent = pUnsorted;

        //get next here, as insertion could change next element
        pUnsorted = pUnsorted->get_next();

        add_entry_to_list(pCurrent);
    }
}



//=======================================================================================
// ColStaffObjsBuilder implementation: algorithm to create a ColStaffObjs
//=======================================================================================
ColStaffObjs* ColStaffObjsBuilder::build(ImoScore* pScore)
{
    ColStaffObjsBuilderEngine* builder = create_builder_engine(pScore);
    ColStaffObjs* pColStaffObjs = builder->do_build();
    pScore->set_staffobjs_table(pColStaffObjs);
    delete builder;
    return pColStaffObjs;
}

//---------------------------------------------------------------------------------------
ColStaffObjsBuilderEngine* ColStaffObjsBuilder::create_builder_engine(ImoScore* pScore)
{
    int major = pScore->get_version_major();
    if (major < 2)
        return LOMSE_NEW ColStaffObjsBuilderEngine1x(pScore);
    else
        return LOMSE_NEW ColStaffObjsBuilderEngine2x(pScore);
}



//=======================================================================================
// ColStaffObjsBuilderEngine
//=======================================================================================
ColStaffObjs* ColStaffObjsBuilderEngine::do_build()
{
    create_table();
    set_num_lines();
    return m_pColStaffObjs;
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine::create_table()
{
    initializations();
    int totalInstruments = m_pImScore->get_num_instruments();
    for (int instr = 0; instr < totalInstruments; instr++)
    {
        create_entries(instr);
        prepare_for_next_instrument();
    }
    collect_anacrusis_info();
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine::collect_anacrusis_info()
{
    ColStaffObjsIterator it = m_pColStaffObjs->begin();
    ImoTimeSignature* pTS = NULL;
    TimeUnits rTime = -1.0;

    //find time signature
    while(it != m_pColStaffObjs->end())
    {
        ImoStaffObj* pSO = (*it)->imo_object();
        if (pSO->is_time_signature())
        {
            pTS = static_cast<ImoTimeSignature*>( pSO );
            break;
        }
        else if (pSO->is_note_rest())
            return;
        ++it;
    }
    if (pTS == NULL)
        return;

    // find first barline
    ++it;
    while(it != m_pColStaffObjs->end())
    {
        ImoStaffObj* pSO = (*it)->imo_object();
        if (pSO->is_barline())
        {
            rTime = (*it)->time();
            break;
        }
        ++it;
    }
    if (rTime <= 0.0)
        return;

    //determine if anacrusis
    TimeUnits measureDuration = pTS->get_measure_duration();
    m_pColStaffObjs->set_anacrusis_missing_time(measureDuration - rTime);
}

//---------------------------------------------------------------------------------------
int ColStaffObjsBuilderEngine::get_line_for(int nVoice, int nStaff)
{
    return m_lines.get_line_assigned_to(nVoice, nStaff);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine::set_num_lines()
{
    m_pColStaffObjs->set_total_lines( m_lines.get_number_of_lines() );
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine::add_entries_for_key_or_time_signature(ImoObj* pImo, int nInstr)
{
    ImoInstrument* pInstr = m_pImScore->get_instrument(nInstr);
    int numStaves = pInstr->get_num_staves();

    ImoStaffObj* pSO = static_cast<ImoStaffObj*>(pImo);
    determine_timepos(pSO);
    for (int nStaff=0; nStaff < numStaves; nStaff++)
    {
        int nLine = get_line_for(0, nStaff);
        m_pColStaffObjs->add_entry(m_nCurMeasure, nInstr, nLine, nStaff, pSO);
    }
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine::prepare_for_next_instrument()
{
    m_lines.new_instrument();
}



//=======================================================================================
// ColStaffObjsBuilderEngine1x implementation: algorithm to create a ColStaffObjs
//=======================================================================================
void ColStaffObjsBuilderEngine1x::initializations()
{
    m_pColStaffObjs = LOMSE_NEW ColStaffObjs();
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::create_entries(int nInstr)
{
    ImoInstrument* pInstr = m_pImScore->get_instrument(nInstr);
    ImoMusicData* pMusicData = pInstr->get_musicdata();
    if (!pMusicData)
        return;

    ImoObj::children_iterator it = pMusicData->begin();
    reset_counters();
    while(it != pMusicData->end())
    {
        if ((*it)->is_go_back_fwd())
        {
            ImoGoBackFwd* pGBF = static_cast<ImoGoBackFwd*>(*it);
            update_time_counter(pGBF);
            ++it;

            //delete_node(pGBF, pMusicData);
            //CSG: goBack/Fwd nodes can not be removed. They are necessary in score edition
            //when ColStaffObjs must be rebuild
        }
        else if ((*it)->is_key_signature() || (*it)->is_time_signature())
        {
            add_entries_for_key_or_time_signature(*it, nInstr);
            ++it;
        }
        else
        {
            ImoStaffObj* pSO = static_cast<ImoStaffObj*>(*it);
            add_entry_for_staffobj(pSO, nInstr);
            update_measure(pSO);
            ++it;
        }
    }
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::add_entry_for_staffobj(ImoObj* pImo, int nInstr)
{
    ImoStaffObj* pSO = static_cast<ImoStaffObj*>(pImo);
    determine_timepos(pSO);
    int nStaff = pSO->get_staff();
    int nVoice = 0;
    if (pSO->is_note_rest())
    {
        ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pSO);
        nVoice = pNR->get_voice();
    }
    int nLine = get_line_for(nVoice, nStaff);
    m_pColStaffObjs->add_entry(m_nCurMeasure, nInstr, nLine, nStaff, pSO);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::delete_node(ImoGoBackFwd* pGBF, ImoMusicData* pMusicData)
{
    pMusicData->remove_child(pGBF);
    delete pGBF;
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::reset_counters()
{
    m_nCurMeasure = 0;
    m_rCurTime = 0.0;
    m_rMaxSegmentTime = 0.0;
    m_rStartSegmentTime = 0.0;
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::determine_timepos(ImoStaffObj* pSO)
{
    pSO->set_time(m_rCurTime);

    if (pSO->is_note())
    {
        ImoNote* pNote = static_cast<ImoNote*>(pSO);
        if (!pNote->is_in_chord() || pNote->is_end_of_chord())
            m_rCurTime += pSO->get_duration();
    }
    else
        m_rCurTime += pSO->get_duration();
    m_rMaxSegmentTime = max(m_rMaxSegmentTime, m_rCurTime);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::update_measure(ImoStaffObj* pSO)
{
    if (pSO->is_barline())
    {
        ++m_nCurMeasure;
        m_rMaxSegmentTime = 0.0;
        m_rStartSegmentTime = m_rCurTime;
    }
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine1x::update_time_counter(ImoGoBackFwd* pGBF)
{
    if (pGBF->is_to_start())
        m_rCurTime = m_rStartSegmentTime;
    else if (pGBF->is_to_end())
        m_rCurTime = m_rMaxSegmentTime;
    else
    {
        m_rCurTime += pGBF->get_time_shift();
        m_rMaxSegmentTime = max(m_rMaxSegmentTime, m_rCurTime);
    }
}

//---------------------------------------------------------------------------------------
ImoSpacer* ColStaffObjsBuilderEngine1x::anchor_object(ImoAuxObj* pAux)
{
    Document* pDoc = m_pImScore->get_the_document();
    ImoSpacer* pAnchor = static_cast<ImoSpacer*>(ImFactory::inject(k_imo_spacer, pDoc));
    pAnchor->add_attachment(pDoc, pAux);
    return pAnchor;
}



//=======================================================================================
// ColStaffObjsBuilderEngine2x implementation: algorithm to create a ColStaffObjs
//=======================================================================================
void ColStaffObjsBuilderEngine2x::initializations()
{
    m_rCurTime.reserve(k_max_voices);
    m_pColStaffObjs = LOMSE_NEW ColStaffObjs();
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine2x::create_entries(int nInstr)
{
    ImoInstrument* pInstr = m_pImScore->get_instrument(nInstr);
    ImoMusicData* pMusicData = pInstr->get_musicdata();
    if (!pMusicData)
        return;

    reset_counters();
    ImoObj::children_iterator it;
    for(it = pMusicData->begin(); it != pMusicData->end(); ++it)
    {
        if ((*it)->is_key_signature() || (*it)->is_time_signature())
        {
            add_entries_for_key_or_time_signature(*it, nInstr);
        }
        else
        {
            ImoStaffObj* pSO = static_cast<ImoStaffObj*>(*it);
            add_entry_for_staffobj(pSO, nInstr);
            if (pSO->is_barline())
                update_measure();
        }
    }
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine2x::add_entry_for_staffobj(ImoObj* pImo, int nInstr)
{
    ImoStaffObj* pSO = static_cast<ImoStaffObj*>(pImo);
    determine_timepos(pSO);
    int nStaff = pSO->get_staff();
    int nVoice = 0;
    if (pSO->is_note_rest())
    {
        ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pSO);
        nVoice = pNR->get_voice();
    }
    int nLine = get_line_for(nVoice, nStaff);
    m_pColStaffObjs->add_entry(m_nCurMeasure, nInstr, nLine, nStaff, pSO);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine2x::reset_counters()
{
    m_nCurMeasure = 0;
    m_rMaxSegmentTime = 0.0;
    m_rStartSegmentTime = 0.0;
    m_rCurTime.assign(k_max_voices, 0.0);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine2x::determine_timepos(ImoStaffObj* pSO)
{
    TimeUnits time;
    TimeUnits duration = pSO->get_duration();

    if (pSO->is_note_rest())
    {
        ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pSO);
        int voice = pNR->get_voice();
        time = m_rCurTime[voice];

        if (pSO->is_note())
        {
            ImoNote* pNote = static_cast<ImoNote*>(pSO);
            if (pNote->is_in_chord() && !pNote->is_end_of_chord())
                duration = 0.0;
        }
        m_rCurTime[voice] += duration;
    }
    else
    {
        time = m_rMaxSegmentTime;
    }

    pSO->set_time(time);
    m_rMaxSegmentTime = max(m_rMaxSegmentTime, time+duration);
}

//---------------------------------------------------------------------------------------
void ColStaffObjsBuilderEngine2x::update_measure()
{
    ++m_nCurMeasure;
    m_rStartSegmentTime = m_rMaxSegmentTime;
    m_rCurTime.assign(k_max_voices, m_rMaxSegmentTime);
}



//=======================================================================================
// StaffVoiceLineTable implementation
//=======================================================================================
StaffVoiceLineTable::StaffVoiceLineTable()
    : m_lastAssignedLine(-1)
{
    assign_line_to(0, 0);

    //first voice in each staff not yet known
    for (int i=0; i < 10; i++)
        m_firstVoiceForStaff.push_back(0);
}

//---------------------------------------------------------------------------------------
int StaffVoiceLineTable::get_line_assigned_to(int nVoice, int nStaff)
{
    int key = form_key(nVoice, nStaff);
    std::map<int, int>::iterator it = m_lineForStaffVoice.find(key);
    if (it != m_lineForStaffVoice.end())
        return it->second;
    else
        return assign_line_to(nVoice, nStaff);
}

//---------------------------------------------------------------------------------------
int StaffVoiceLineTable::assign_line_to(int nVoice, int nStaff)
{
    int key = form_key(nVoice, nStaff);
    if (nVoice != 0)
    {
        if (m_firstVoiceForStaff[nStaff] == 0)
        {
            //No voice yet assigned to this staff. Save voice nVoice as
            //first voice in this staff and assign it the sSame line than
            //voice 0 (nStaff)
            m_firstVoiceForStaff[nStaff] = nVoice;
            int line = get_line_assigned_to(0, nStaff);
            m_lineForStaffVoice[key] = line;
            return line;
        }
        else if (m_firstVoiceForStaff[nStaff] == nVoice)
        {
            //voice nVoice is the first voice found in this staff.
            //Assign it the sSame line than voice 0 (nStaff)
            return get_line_assigned_to(0, nStaff);
        }
        //else, assig it a line
    }

    //voice == 0 or voice is not first voice for this staff.
    //assign it the next available line number
    int line = ++m_lastAssignedLine;
    m_lineForStaffVoice[key] = line;
    return line;
}

//---------------------------------------------------------------------------------------
void StaffVoiceLineTable::new_instrument()
{
    m_lineForStaffVoice.clear();

    assign_line_to(0, 0);

    //first voice in each staff not yet known
    for (int i=0; i < 10; i++)
        m_firstVoiceForStaff[i] = 0;
}


//=======================================================================================
// ScoreAlgorithms implementation
//=======================================================================================
ImoNote* ScoreAlgorithms::find_possible_end_of_tie(ColStaffObjs* pColStaffObjs,
                                                   ImoNote* pStartNote)
{
    // This method explores forwards to try to find a note ("the candidate note") that
    // can be tied (as end of tie) with pStartNote.
    //
    // Algorithm:
    // Find the first comming note of the same pitch and voice, and verify that
    // distance (in timepos) is equal to start note duration.
    // The search will fail as soon as we find a rest or a note with different pitch.

    //get target pitch and voice
    FPitch pitch = pStartNote->get_fpitch();
    int voice = pStartNote->get_voice();

    //define a forwards iterator and find start note
    ColStaffObjsIterator it = pColStaffObjs->find(pStartNote);
    if (it == pColStaffObjs->end())
        return NULL;    //pStartNote not found ??????

    int instr = (*it)->num_instrument();

    //do search
    ++it;
    while(it != pColStaffObjs->end())
    {
        ImoStaffObj* pSO = (*it)->imo_object();
        if ((*it)->num_instrument() == instr
            && pSO->is_note_rest()
            && static_cast<ImoNoteRest*>(pSO)->get_voice() == voice)
        {
            if (pSO->is_note())
            {
                if (static_cast<ImoNote*>(pSO)->get_fpitch() == pitch)
                    return static_cast<ImoNote*>(pSO);    // candidate found
                else
                    // a note in the same voice with different pitch found.
                    // Imposible to tie
                    return NULL;
            }
            else
                // a rest in the same voice found. Imposible to tie
                return NULL;
        }
        ++it;
    }
    return NULL;        //no suitable note found
}

//---------------------------------------------------------------------------------------
int ScoreAlgorithms::get_applicable_clef_for(ImoScore* pScore,
                                             int iInstr, int iStaff, TimeUnits time)
{
    ColStaffObjs* pColStaffObjs = pScore->get_staffobjs_table();
    int clef = k_clef_undefined;
    ColStaffObjsIterator it;
    for (it=pColStaffObjs->begin(); it != pColStaffObjs->end(); ++it)
    {
        if (is_greater_time((*it)->time(), time))
            break;
        if ((*it)->num_instrument() == iInstr && (*it)->staff() == iStaff)
        {
            ImoObj* pImo = (*it)->imo_object();
            if (pImo->is_clef())
                clef = static_cast<ImoClef*>(pImo)->get_clef_type();
        }
    }
    return clef;
}

//---------------------------------------------------------------------------------------
ImoNoteRest* ScoreAlgorithms::find_noterest_at(ImoScore* pScore,
                                               int instr, int voice, TimeUnits time)
{
    ColStaffObjs* pColStaffObjs = pScore->get_staffobjs_table();
    ColStaffObjsIterator it;
    for (it=pColStaffObjs->begin(); it != pColStaffObjs->end(); ++it)
    {
        if (is_greater_time((*it)->time(), time))
            break;
        ImoObj* pImo = (*it)->imo_object();
        if (pImo->is_note_rest())
        {
            ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pImo);
            if ((*it)->num_instrument() == instr)
            {
                if (pNR->get_voice() == voice
                    && !is_greater_time(time, (*it)->time() + pNR->get_duration()) )
                    return pNR;
            }
        }
    }
    return NULL;
}

//---------------------------------------------------------------------------------------
list<OverlappedNoteRest*> ScoreAlgorithms::find_and_classify_overlapped_noterests_at(
        ImoScore* pScore, int instr, int voice, TimeUnits time, TimeUnits duration)
{
    list<OverlappedNoteRest*> overlaps;
    ColStaffObjs* pColStaffObjs = pScore->get_staffobjs_table();
    ColStaffObjsIterator it;
    for (it=pColStaffObjs->begin(); it != pColStaffObjs->end(); ++it)
    {
        ImoObj* pImo = (*it)->imo_object();
        if (pImo->is_note_rest())
        {
            ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pImo);
            TimeUnits nrTime = (*it)->time();
            TimeUnits nrDuration = pNR->get_duration();
            if ((*it)->num_instrument() == instr
                && pNR->get_voice() == voice
                && is_greater_time(time + duration, nrTime)     //starts before end of inserted one
                && is_lower_time(time, nrTime + nrDuration)     //ends after start of inserted one
               )
            {
                OverlappedNoteRest* pOV = LOMSE_NEW OverlappedNoteRest(pNR);
                if (is_equal_time(nrTime, time))
                {
                    //both start at same time
                    if (is_lower_time(duration, nrDuration))
                    {
                        //test 4
                        pOV->type = k_overlap_at_start;
                        pOV->overlap = duration;
                    }
                    else
                    {
                        //test 1
                        pOV->type = k_overlap_full;
                        pOV->overlap = nrDuration;
                    }
                }
                else if (is_lower_time(time, nrTime))
                {
                    //starts after inserted one: overlap at_start or full
                    pOV->overlap = duration - (nrTime - time);
                    if (is_lower_time(pOV->overlap, nrDuration))
                    {
                        //test 5
                        pOV->type = k_overlap_at_start;
                    }
                    else
                    {
                        //test 3
                        pOV->type = k_overlap_full;
                        pOV->overlap = nrDuration;
                    }
                }
                else
                {
                    //starts before inserted one: overlap at_end
                    //test 2, 3, 5
                    pOV->overlap = nrDuration - (time - nrTime);
                    pOV->type = k_overlap_at_end;
                }

                overlaps.push_back(pOV);
            }
            else if (is_lower_time(time + duration, nrTime))
                break;
        }
    }
    return overlaps;
}

//---------------------------------------------------------------------------------------
TimeUnits ScoreAlgorithms::find_end_time_for_voice(ImoScore* pScore,
                                        int instr, int voice, TimeUnits maxTime)
{

    ColStaffObjs* pColStaffObjs = pScore->get_staffobjs_table();
    ColStaffObjsIterator it =
                        find_barline_with_time_lower_or_equal(pScore, instr, maxTime);

    TimeUnits endTime = 0.0;
    if (it != pColStaffObjs->end())
        endTime = (*it)->time();

    for (; it != pColStaffObjs->end(); ++it)
    {
        ImoObj* pImo = (*it)->imo_object();
        if (pImo->is_note_rest())
        {
            ImoNoteRest* pNR = static_cast<ImoNoteRest*>(pImo);
            TimeUnits time = (*it)->time();
            TimeUnits duration = pNR->get_duration();
            if ((*it)->num_instrument() == instr && pNR->get_voice() == voice)
            {
                if (!is_greater_time(time + duration, maxTime))
                    endTime = max(endTime, time+duration);
            }

            if (is_greater_time(time, maxTime))
                break;
        }
    }
    return endTime;
}

//---------------------------------------------------------------------------------------
ColStaffObjsIterator ScoreAlgorithms::find_barline_with_time_lower_or_equal(
            ImoScore* pScore, int UNUSED(instr), TimeUnits maxTime)
{
    ColStaffObjs* pColStaffObjs = pScore->get_staffobjs_table();
    ColStaffObjsIterator it = pColStaffObjs->begin();
    ColStaffObjsIterator itLastBarline = it;
    for (; it != pColStaffObjs->end(); ++it)
    {
        ImoObj* pImo = (*it)->imo_object();
        if (pImo->is_barline())
        {
            if (is_greater_time((*it)->time(), maxTime))
                break;
            itLastBarline = it;
        }
    }
    return itLastBarline;
}


}  //namespace lomse
