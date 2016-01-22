#if !defined(AFX_AUTOLOCK_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
#define AFX_AUTOLOCK_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

__inline VOID _Track(LPCTSTR ptzFormat, ...)
{
	va_list vlArgs;
	TCHAR tzText[1024];
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	OutputDebugString(tzText);
	va_end(vlArgs);
}

//stdafx.h中定义#define _WIN32_WINNT 0x0500，不然出错
class CriticalSection
{
private:
	CRITICAL_SECTION m_cs;
	volatile bool m_state;
	int count;
public:
	CriticalSection(int spinCount = 0) : m_state(true)
	{
		if(!InitializeCriticalSectionAndSpinCount(&m_cs, spinCount))
			m_state = false;
		count = 0;
	}

	~CriticalSection(void)
	{
		DeleteCriticalSection(&m_cs);
	}

	bool Lock()
	{
		if(!m_state) return false;
		EnterCriticalSection(&m_cs);
		count++;
		_Track("Lock%d",count);
		return true;
	}

	bool UnLock()
	{
		if(!m_state) return false;
		LeaveCriticalSection(&m_cs);
		_Track("UnLock%d",count);
		return true;
	}
};

class CAutoLock
{
public:
	CAutoLock(CriticalSection& cs) 
		: m_rCs(cs)
	{ 
		m_rCs.Lock(); 
	}

	~CAutoLock()
	{ 
		m_rCs.UnLock(); 
	}
	
private:
	CriticalSection& m_rCs;
};

#endif // !defined(AFX_AUTOLOCK_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)