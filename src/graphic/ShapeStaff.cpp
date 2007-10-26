//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ShapeStaff.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "GMObject.h"
#include "../score/Score.h"

#include "ShapeStaff.h"


//-------------------------------------------------------------------------------------
// Implementation of lmShapeStaff: an staff (usually 5 lines)


lmShapeStaff::lmShapeStaff(lmStaff* pStaff, int nNumLines, lmLUnits uLineWidth,
						   lmLUnits uSpacing, lmLUnits xLeft, lmLUnits yTop,
						   lmLUnits xRight, wxColour color)
	: lmSimpleShape(eGMO_ShapeStaff, pStaff)
{
	m_nNumLines = nNumLines;
	m_uLineWidth = uLineWidth;
	m_uSpacing = uSpacing;
	m_color = color;
	
	//bounds
	SetXLeft(xLeft);
	SetYTop(yTop);
	SetXRight(xRight);
	SetYBottom(yTop + nNumLines * uLineWidth + (nNumLines - 1) * uSpacing);

}

lmShapeStaff::~lmShapeStaff()
{
}

void lmShapeStaff::Render(lmPaper* pPaper, lmUPoint uPos, wxColour color)
{
    //draw the staff
	lmLUnits yPos = m_uBoundsTop.y;
    for (int iL=0; iL < m_nNumLines; iL++ )
	{
        pPaper->SolidLine(m_uBoundsTop.x, yPos, m_uBoundsBottom.x, yPos,
                          m_uLineWidth, eEdgeNormal, m_color);
        yPos += m_uSpacing;
    }
}

