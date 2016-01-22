#ifndef _THREADTEMPLATE_H__
#define _THREADTEMPLATE_H__

namespace ThreadTemplate
{
	template<typename xClass, typename xReturn>
	class xBind1
	{
		typedef xReturn (xClass::* CallFuc)();
		CallFuc m_CallFuc;
		xClass* m_pThis;
		
	public:
		xBind1(xClass* pThis, CallFuc fuc)
		{
			m_pThis   = pThis;
			m_CallFuc = fuc;
		}
		xReturn Run()
		{
			return ((*m_pThis).*(m_CallFuc))();
		}
	};

	template<typename xClass, typename xReturn, typename xParam>
	class xBind2
	{
		typedef xReturn (xClass::* CallFuc)(xParam);
		CallFuc m_CallFuc;
		xClass* m_pThis;
		xParam  m_Param;

	public:
		xBind2(xClass* pThis, CallFuc fuc, xParam param)
		{
			m_pThis   = pThis;
			m_CallFuc = fuc;
			m_Param   = param;			
		}
		xReturn Run()
		{
			return ((*m_pThis).*(m_CallFuc))(m_Param);
		}
	};

	template<typename xClass, typename xReturn>
	class xThread1
	{
	public:
		static DWORD WINAPI Thread(LPVOID lp)
		{
			typedef xBind1<xClass, xReturn> bind;
			bind* pThis = (bind*)lp;
			pThis->Run();
			delete pThis;
			return 0;
		}
	};

	template<typename xClass, typename xReturn, typename xParam>
	class xThread2
	{
	public:
		static DWORD WINAPI Thread(LPVOID lp)
		{
			typedef xBind2<xClass, xReturn, xParam> bind;
			bind* pThis = (bind*)lp;
			pThis->Run();
			delete pThis;
			return 0;
		}
	};
	
	template<typename xClass, typename xReturn>
	inline HANDLE StartThread(xClass* pThis, xReturn (xClass::* pfn)())
	{
		xBind1<xClass, xReturn>* pbin = new xBind1<xClass, xReturn>(pThis,pfn);
		return CreateThread(0, 0, xThread1<xClass, xReturn>::Thread,pbin,0, 0);
	}

	template<typename xClass, typename xReturn, typename xParam>
	inline HANDLE StartThread(xClass* pThis, xReturn (xClass::* pfn)(xParam), xParam lp)
	{
		xBind2<xClass, xReturn, xParam>* pbin = new xBind2<xClass, xReturn, xParam>(pThis,pfn, lp);
		return CreateThread(0, 0, xThread2<xClass, xReturn, xParam>::Thread,pbin,0, 0);
	}
}//end namespace 

#endif //_THREADTEMPLATE_H__
