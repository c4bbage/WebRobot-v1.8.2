// XNamedColors.cpp  Version 1.5
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// This software is released into the public domain.  You are free to use it
// in any way you like, except that you may not sell this source code.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this software
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

// NOTE ABOUT PRECOMPILED HEADERS:
// This file does not need to be compiled with precompiled headers (.pch).
// To disable this, go to Project | Settings | C/C++ | Precompiled Headers
// and select "Not using precompiled headers".  Be sure to do this for all
// build configurations.
//#include "stdafx.h"
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include "XNamedColors.h"

#pragma warning(disable : 4127)	// conditional expression is constant (_ASSERTE)
#pragma warning(disable : 4996)	// disable bogus deprecation warning


///////////////////////////////////////////////////////////////////////////////
// array of colors and names
const CXNamedColors::COLORNAMES CXNamedColors::m_aColorNames[] = 
{
	// named HTML colors
	{ colorAliceBlue,				_T("AliceBlue") },
	{ colorAntiqueWhite,			_T("AntiqueWhite") },
	{ colorAqua,					_T("Aqua") },
	{ colorAquamarine,				_T("Aquamarine") },
	{ colorAzure,					_T("Azure") },
	{ colorBeige,					_T("Beige") },
	{ colorBisque,					_T("Bisque") },
	{ colorBlack,					_T("Black") },
	{ colorBlanchedAlmond,			_T("BlanchedAlmond") },
	{ colorBlue,					_T("Blue") },
	{ colorBlueViolet,				_T("BlueViolet") },
	{ colorBrown,					_T("Brown") },
	{ colorBurlywood,				_T("Burlywood") },
	{ colorCadetBlue,				_T("CadetBlue") },
	{ colorChartreuse,				_T("Chartreuse") },
	{ colorChocolate,				_T("Chocolate") },
	{ colorCoral,					_T("Coral") },
	{ colorCornflowerBlue,			_T("CornflowerBlue") },
	{ colorCornsilk,				_T("Cornsilk") },
	{ colorCrimson,					_T("Crimson") },
	{ colorCyan,					_T("Cyan") },
	{ colorDarkBlue,				_T("DarkBlue") },
	{ colorDarkCyan,				_T("DarkCyan") },
	{ colorDarkGoldenRod,			_T("DarkGoldenRod") },
	{ colorDarkGray,				_T("DarkGray") },
	{ colorDarkGreen,				_T("DarkGreen") },
	{ colorDarkKhaki,				_T("DarkKhaki") },
	{ colorDarkMagenta,				_T("DarkMagenta") },
	{ colorDarkOliveGreen,			_T("DarkOliveGreen") },
	{ colorDarkOrange,				_T("DarkOrange") },
	{ colorDarkOrchid,				_T("DarkOrchid") },
	{ colorDarkRed,					_T("DarkRed") },
	{ colorDarkSalmon,				_T("DarkSalmon") },
	{ colorDarkSeaGreen,			_T("DarkSeaGreen") },
	{ colorDarkSlateBlue,			_T("DarkSlateBlue") },
	{ colorDarkSlateGray,			_T("DarkSlateGray") },
	{ colorDarkTurquoise,			_T("DarkTurquoise") },
	{ colorDarkViolet,				_T("DarkViolet") },
	{ colorDeepPink,				_T("DeepPink") },
	{ colorDeepSkyBlue,				_T("DeepSkyBlue") },
	{ colorDimGray,					_T("DimGray") },
	{ colorDodgerBlue,				_T("DodgerBlue") },
	{ colorFireBrick,				_T("FireBrick") },
	{ colorFloralWhite,				_T("FloralWhite") },
	{ colorForestGreen,				_T("ForestGreen") },
	{ colorFuchsia,					_T("Fuchsia") },
	{ colorGainsboro,				_T("Gainsboro") },
	{ colorGhostWhite,				_T("GhostWhite") },
	{ colorGold,					_T("Gold") },
	{ colorGoldenRod,				_T("GoldenRod") },
	{ colorGray,					_T("Gray") },
	{ colorGreen,					_T("Green") },
	{ colorGreenYellow,				_T("GreenYellow") },
	{ colorHoneyDew,				_T("HoneyDew") },
	{ colorHotPink,					_T("HotPink") },
	{ colorIndianRed,				_T("IndianRed") },
	{ colorIndigo,					_T("Indigo") },
	{ colorIvory,					_T("Ivory") },
	{ colorKhaki,					_T("Khaki") },
	{ colorLavender,				_T("Lavender") },
	{ colorLavenderBlush,			_T("LavenderBlush") },
	{ colorLawngreen,				_T("Lawngreen") },
	{ colorLemonChiffon,			_T("LemonChiffon") },
	{ colorLightBlue,				_T("LightBlue") },
	{ colorLightCoral,				_T("LightCoral") },
	{ colorLightCyan,				_T("LightCyan") },
	{ colorLightGoldenRodYellow,	_T("LightGoldenRodYellow")},
	{ colorLightGreen,				_T("LightGreen") },
	{ colorLightGrey,				_T("LightGrey") },
	{ colorLightPink,				_T("LightPink") },
	{ colorLightSalmon,				_T("LightSalmon") },
	{ colorLightSeaGreen,			_T("LightSeaGreen") },
	{ colorLightSkyBlue,			_T("LightSkyBlue") },
	{ colorLightSlateGray,			_T("LightSlateGray") },
	{ colorLightSteelBlue,			_T("LightSteelBlue") },
	{ colorLightYellow,				_T("LightYellow") },
	{ colorLime,					_T("Lime") },
	{ colorLimeGreen,				_T("LimeGreen") },
	{ colorLinen,					_T("Linen") },
	{ colorMagenta,					_T("Magenta") },
	{ colorMaroon,					_T("Maroon") },
	{ colorMediumAquamarine,		_T("MediumAquamarine") },
	{ colorMediumBlue,				_T("MediumBlue") },
	{ colorMediumOrchid,			_T("MediumOrchid") },
	{ colorMediumPurple,			_T("MediumPurple") },
	{ colorMediumSeaGreen,			_T("MediumSeaGreen") },
	{ colorMediumSlateBlue,			_T("MediumSlateBlue") },
	{ colorMediumSpringGreen,		_T("MediumSpringGreen") },
	{ colorMediumTurquoise,			_T("MediumTurquoise") },
	{ colorMediumVioletRed,			_T("MediumVioletRed") },
	{ colorMidnightBlue,			_T("MidnightBlue") },
	{ colorMintCream,				_T("MintCream") },
	{ colorMistyRose,				_T("MistyRose") },
	{ colorMoccasin,				_T("Moccasin") },
	{ colorNavajoWhite,				_T("NavajoWhite") },
	{ colorNavy,					_T("Navy") },
	{ colorOldLace,					_T("OldLace") },
	{ colorOlive,					_T("Olive") },
	{ colorOliveDrab,				_T("OliveDrab") },
	{ colorOrange,					_T("Orange") },
	{ colorOrangeRed,				_T("OrangeRed") },
	{ colorOrchid,					_T("Orchid") },
	{ colorPaleGoldenRod,			_T("PaleGoldenRod") },
	{ colorPaleGreen,				_T("PaleGreen") },
	{ colorPaleTurquoise,			_T("PaleTurquoise") },
	{ colorPaleVioletRed,			_T("PaleVioletRed") },
	{ colorPapayaWhip,				_T("PapayaWhip") },
	{ colorPeachPuff,				_T("PeachPuff") },
	{ colorPeru,					_T("Peru") },
	{ colorPink,					_T("Pink") },
	{ colorPlum,					_T("Plum") },
	{ colorPowderBlue,				_T("PowderBlue") },
	{ colorPurple,					_T("Purple") },
	{ colorRed,						_T("Red") },
	{ colorRosyBrown,				_T("RosyBrown") },
	{ colorRoyalBlue,				_T("RoyalBlue") },
	{ colorSaddleBrown,				_T("SaddleBrown") },
	{ colorSalmon,					_T("Salmon") },
	{ colorSandyBrown,				_T("SandyBrown") },
	{ colorSeaGreen,				_T("SeaGreen") },
	{ colorSeaShell,				_T("SeaShell") },
	{ colorSienna,					_T("Sienna") },
	{ colorSilver,					_T("Silver") },
	{ colorSkyBlue,					_T("SkyBlue") },
	{ colorSlateBlue,				_T("SlateBlue") },
	{ colorSlateGray,				_T("SlateGray") },
	{ colorSnow,					_T("Snow") },
	{ colorSpringGreen,				_T("SpringGreen") },
	{ colorSteelBlue,				_T("SteelBlue") },
	{ colorTan,						_T("Tan") },
	{ colorTeal,					_T("Teal") },
	{ colorThistle,					_T("Thistle") },
	{ colorTomato,					_T("Tomato") },
	{ colorTurquoise,				_T("Turquoise") },
	{ colorViolet,					_T("Violet") },
	{ colorWheat,					_T("Wheat") },
	{ colorWhite,					_T("White") },
	{ colorWhiteSmoke,				_T("WhiteSmoke") },
	{ colorYellow,					_T("Yellow") },
	{ colorYellowGreen,				_T("YellowGreen") },

	// Windows system colors
	{ colorActiveBorder,			_T("ActiveBorder") },
	{ colorActiveCaption,			_T("ActiveCaption") },
	{ colorActiveCaptionText,		_T("ActiveCaptionText") },
	{ colorAppWorkspace,			_T("AppWorkspace") },
	{ colorBackground,				_T("Background") },
	{ colorBtnFace,					_T("BtnFace") },
	{ colorBtnHighlight,			_T("BtnHighlight") },
	{ colorBtnHilight,				_T("BtnHilight") },
	{ colorBtnShadow,				_T("BtnShadow") },
	{ colorBtnText,					_T("BtnText") },
	{ colorCaptionText,				_T("CaptionText") },
	{ colorControl,					_T("Control") },
	{ colorControlDark,				_T("ControlDark") },
	{ colorControlDarkDark,			_T("ControlDarkDark") },
	{ colorControlLight,			_T("ControlLight") },
	{ colorControlLightLight,		_T("ControlLightLight") },
	{ colorControlText,				_T("ControlText") },
	{ colorDesktop,					_T("Desktop") },
	{ colorGradientActiveCaption,	_T("GradientActiveCaption") },
	{ colorGradientInactiveCaption,	_T("GradientInactiveCaption") },
	{ colorGrayText,				_T("GrayText") },
	{ colorHighlight,				_T("Highlight") },
	{ colorHighlightText,			_T("HighlightText") },
	{ colorHotLight,				_T("HotLight") },
	{ colorHotTrack,				_T("HotTrack") },
	{ colorInactiveBorder,			_T("InactiveBorder") },
	{ colorInactiveCaption,			_T("InactiveCaption") },
	{ colorInactiveCaptionText,		_T("InactiveCaptionText") },
	{ colorInfo,					_T("Info") },
	{ colorInfoBk,					_T("InfoBk") },
	{ colorInfoText,				_T("InfoText") },
	{ colorMenu,					_T("Menu") },
	{ colorMenuBar,					_T("MenuBar") },
	{ colorMenuHilight,				_T("MenuHilight") },
	{ colorMenuText,				_T("MenuText") },
	{ colorScrollBar,				_T("ScrollBar") },
	{ colorWindow,					_T("Window") },
	{ colorWindowFrame,				_T("WindowFrame") },
	{ colorWindowText,				_T("WindowText") }
};

const int CXNamedColors::m_nNamedColors = 
				sizeof(CXNamedColors::m_aColorNames) / 
				sizeof(CXNamedColors::m_aColorNames[0]);

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::CXNamedColors()
{
	m_Color = RGB(0,0,0);		// initialize to black
}

///////////////////////////////////////////////////////////////////////////////
// can be:  "red" or "255,0,0" or "#0000FF"
CXNamedColors::CXNamedColors(LPCTSTR lpszColor)
{
	SetColorFromString(lpszColor);
}

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::CXNamedColors(COLORREF color)
{
	m_Color = color;
}

///////////////////////////////////////////////////////////////////////////////
// nSysColorIndex is one of the values used with GetSysColor()
CXNamedColors::CXNamedColors(int nSysColorIndex)
{
	SetSysColor(nSysColorIndex);
}

///////////////////////////////////////////////////////////////////////////////
CXNamedColors::~CXNamedColors()
{
}

///////////////////////////////////////////////////////////////////////////////
int CXNamedColors::GetColorIndex()
{
	int rc = -1;

	for (int i = 0; i < m_nNamedColors; i++)
	{
		if (m_Color == m_aColorNames[i].color)
		{
			rc = i;
			break;
		}
	}

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CXNamedColors::GetColorByIndex(int index)
{
	_ASSERTE((index >= 0) && (index < m_nNamedColors));

	COLORREF rc = (DWORD)-1;

	if ((index >= 0) && (index < m_nNamedColors))
	{
		rc = m_aColorNames[index].color;
		if (rc & 0x80000000L)
			rc = GetSysColor(rc & 0x7FFFFFFFL);
	}

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::GetColorNameByIndex(int index, 
										LPTSTR lpszBuf, 
										DWORD nBufSize)
{
	_ASSERTE(lpszBuf);
	_ASSERTE(nBufSize > 0);
	_ASSERTE((index >= 0) && (index < m_nNamedColors));
	if (!lpszBuf || (nBufSize == 0))
		return;

	if ((index >= 0) && (index < m_nNamedColors))
	{
		memset(lpszBuf, 0, nBufSize*sizeof(TCHAR));
		_tcsncpy(lpszBuf, m_aColorNames[index].pszName, nBufSize-1);
	}
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::GetHex(LPTSTR lpszBuf, DWORD nBufSize)	// nBufSize in TCHARs
{
	_ASSERTE(lpszBuf);
	_ASSERTE(nBufSize > 0);
	if (!lpszBuf || (nBufSize == 0))
		return;

	memset(lpszBuf, 0, nBufSize*sizeof(TCHAR));

	BYTE r = GetRValue(GetRGB());
	BYTE g = GetGValue(GetRGB());
	BYTE b = GetBValue(GetRGB());

	_sntprintf(lpszBuf, nBufSize-1, _T("#%02X%02X%02X"), r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
COLORREF CXNamedColors::GetRGB() 
{ 
	COLORREF rc = m_Color;
	if (IsSystemColor())
		rc = GetSysColor(m_Color & 0x7FFFFFFFL);
	return rc;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::GetName(LPTSTR lpszBuf, DWORD nBufSize)	// nBufSize in TCHARs
{
	_ASSERTE(lpszBuf);
	_ASSERTE(nBufSize > 0);
	if (!lpszBuf || (nBufSize == 0))
		return;

	memset(lpszBuf, 0, nBufSize*sizeof(TCHAR));

	for (int i = 0; i < m_nNamedColors; i++)
	{
		if (m_Color == m_aColorNames[i].color)
		{
			_tcsncpy(lpszBuf, m_aColorNames[i].pszName, nBufSize-1);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::GetRGBString(LPTSTR lpszBuf, DWORD nBufSize) // nBufSize in TCHARs
{
	_ASSERTE(lpszBuf);
	_ASSERTE(nBufSize > 0);
	if (!lpszBuf || (nBufSize == 0))
		return;

	memset(lpszBuf, 0, nBufSize*sizeof(TCHAR));

	_sntprintf(lpszBuf, nBufSize-1, _T("%u,%u,%u"), GetR(), GetG(), GetB());
}

///////////////////////////////////////////////////////////////////////////////
// #RRGGBB
void CXNamedColors::SetHex(LPCTSTR lpszHex)
{
	_ASSERTE(lpszHex);
	if (!lpszHex)
		return;

	COLORREF rgb = RGB(0,0,0);
	LPCTSTR cp = lpszHex;

	if ((*cp == _T('#')) && (_tcslen(lpszHex) == 7))
	{
		TCHAR s[3] = { _T('\0') };
		cp++;

		s[0] = *cp++;
		s[1] = *cp++;
		BYTE r = (BYTE)_tcstoul(s, NULL, 16);

		s[0] = *cp++;
		s[1] = *cp++;
		BYTE g = (BYTE)_tcstoul(s, NULL, 16);

		s[0] = *cp++;
		s[1] = *cp++;
		BYTE b = (BYTE)_tcstoul(s, NULL, 16);

		rgb = RGB(r,g,b);
	}
	m_Color = rgb;
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::SetName(LPCTSTR lpszColorName)
{
	_ASSERTE(lpszColorName);
	if (!lpszColorName)
		return;

	COLORREF rgb = RGB(0,0,0);

	for (int i = 0; i < m_nNamedColors; i++)
	{
		if (_tcsicmp(lpszColorName, m_aColorNames[i].pszName) == 0)
		{
			rgb = m_aColorNames[i].color;
			break;
		}
	}
	m_Color = rgb;
}

///////////////////////////////////////////////////////////////////////////////
// lpszColor:  "red" or "255,0,0" or "#0000FF"
void CXNamedColors::SetColorFromString(LPCTSTR lpszColor)
{
	_ASSERTE(lpszColor);
	if (!lpszColor)
		return;

	m_Color = RGB(0,0,0);		// initialize to black
	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;
	TCHAR *cp = 0;

	if ((cp = (TCHAR*)_tcschr(lpszColor, _T(','))) != NULL)
	{
		// "255,0,0"
		r = (BYTE) _ttoi(lpszColor);
		cp++;
		g = (BYTE) _ttoi(cp);
		cp = _tcschr(cp, _T(','));
		if (cp)
		{
			cp++;
			b = (BYTE) _ttoi(cp);
		}
		m_Color = RGB(r,g,b);
	}
	else if ((cp = (TCHAR*)_tcschr(lpszColor, _T('#'))) != NULL)
	{
		// "#0000FF"
		if (_tcslen(lpszColor) == 7)
		{
			TCHAR s[3] = { _T('\0') };
			cp++;
			s[0] = *cp++;
			s[1] = *cp++;
			r = (BYTE)_tcstoul(s, NULL, 16);
			s[0] = *cp++;
			s[1] = *cp++;
			g = (BYTE)_tcstoul(s, NULL, 16);
			s[0] = *cp++;
			s[1] = *cp++;
			b = (BYTE)_tcstoul(s, NULL, 16);
			m_Color = RGB(r,g,b);
		}
	}
	else
	{
		// "red"
		for (int i = 0; i < m_nNamedColors; i++)
		{
			if (_tcsicmp(lpszColor, m_aColorNames[i].pszName) == 0)
			{
				m_Color = m_aColorNames[i].color;
				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CXNamedColors::SetRGB(BYTE r, BYTE g, BYTE b) 
{ 
	m_Color = RGB(r, g, b);
}

///////////////////////////////////////////////////////////////////////////////
// nSysColorIndex is one of the values used with GetSysColor()
void CXNamedColors::SetSysColor(int nSysColorIndex)
{
	_ASSERTE((nSysColorIndex >= 0) && (nSysColorIndex <= COLOR_MENUBAR));

	if ((nSysColorIndex >= 0) && (nSysColorIndex <= COLOR_MENUBAR))
		m_Color = (DWORD) (nSysColorIndex | 0x80000000);
}

#ifdef _DEBUG
///////////////////////////////////////////////////////////////////////////////
//
// Dumps the color table in a form suitable for a HTML table, e.g.:
//
// <tr>
// <td style="background-color:#F0F8FF; color:black" align="center">AliceBlue<br>(#F0F8FF)</td>
// <td style="background-color:#FAEBD7; color:black" align="center">AntiqueWhite<br>(#FAEBD7)</td>
// <td style="background-color:#00FFFF; color:black" align="center">Aqua<br>(#00FFFF)</td>
// <td style="background-color:#7FFFD4; color:black" align="center">Aquamarine<br>(#7FFFD4)</td>
// <td style="background-color:#F0FFFF; color:black" align="center">Azure<br>(#F0FFFF)</td>
// </tr>
//
void CXNamedColors::Dump(LPCTSTR lpszFile)
{
	_ASSERTE(lpszFile);
	if (!lpszFile)
		return;

	FILE * f = _tfopen(lpszFile, _T("w"));
	if (f)
	{
		_ftprintf(f, _T("<html><body>\n"));
		_ftprintf(f, _T("<center><table bgcolor=\"gray\" border=1 cellspacing=1 cellpadding=3 summary=\"named colors\">"));

		TCHAR szBuf[100];
		for (int j = 0; j < m_nNamedColors; )
		{
			_ftprintf(f, _T("<tr>\n"));
			int cols = (j >= 140) ? 4 : 5;
			for (int k = 0; k < cols; k++)
			{
				if (m_aColorNames[j].color & 0x80000000)
				{
					DWORD index = m_aColorNames[j].color;
					index = index & 0x7FFFFFFF;
					SetSysColor(index);
				}
				else
				{
					SetRGB(m_aColorNames[j].color);
				}
				GetHex(szBuf, 99);
				TCHAR *text = _T("black");
				if (_tcscmp(szBuf, _T("#000000")) == 0)
					text = _T("white");
				_ftprintf(f, 
					_T("<td style=\"background-color:%s; color:%s\" align=\"center\">%s<br>(%s)</td>\n"),
					szBuf,
					text,
					CXNamedColors::m_aColorNames[j].pszName, 
					szBuf);
				j++;
				if (j >= CXNamedColors::m_nNamedColors)
					break;
			}
			_ftprintf(f, _T("</tr>\n\n"));
		}
		_ftprintf(f, _T("</table></center></body></html>\n"));
		fclose(f);
	}
}
#endif
