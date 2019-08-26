//---------------------------------------------------------------------------
// This file is part of the Medialooks MPlatform 
//
// MClock.h 
//
//---------------------------------------------------------------------------
// Copyright (c), Medialooks Soft OOO
// http://www.medialooks.com/support/
//
//---------------------------------------------------------------------------
//
// This file and the source code contained within is Intellectual Property
// of Medialooks Soft OOO. Your use of this source code is subject to
// the Medialooks End-User License Agreement that can be found
// here: http://www.medialooks.com/la/.
//
// 2017/06/10 Dl
//---------------------------------------------------------------------------

#pragma once

#include "../Devices/MTimer.h"
#include "../Base/MHelpers.h"
#include "../DSTL/ippMerge.h"
#include "../Helpers/MStat.h"

#pragma warning(push)
#pragma warning(disable:4189)

class MClockOuter : 
	public MClock
{
	double				m_dblSpeedUp;
	MClock				m_clockSystem;
	REFERENCE_TIME		m_rtClockOffset;
	double	m_dblLowLimit;
	double	m_dblUpLimit;


	// For buffers correction
	MAverageCalc::TPtr	m_pBufferAvg;
	MAverageCalc::TPtr	m_pBufferAvg2;
public:

	MClockOuter(double _dblSpeedUp = 1.0)
		: m_dblSpeedUp( MAX(0.00001, _dblSpeedUp) ),
		m_pBufferAvg( NULL ), m_pBufferAvg2(NULL)
	{
		m_clockSystem.SpeedUp_Set(m_dblSpeedUp);
	}

	~MClockOuter()
	{
		_DestroyBufferAvg();
	}

	double OnTime(M_TIME* _pTimeIn)
	{
		ATLASSERT(_pTimeIn);
		if (!_pTimeIn) return -1.0;

		if (BUFFER_POS_HAVE(_pTimeIn->eFFlags))
		{
			if (!_CreateBufferAvg())
				return -1.0;

			double dblBufferPos = BUFFER_POS_GET(_pTimeIn->eFFlags);

			m_pBufferAvg->AddValue()
		}
		else
		{
			_DestroyBufferAvg();
		}
	}


	REFERENCE_TIME CorrectedTime(M_TIME* _pTimeIn, REFERENCE_TIME _rtReceiveTime = MINLONGLONG )
	{
		return E_NOTIMPL;
	}


private:
	bool _CreateBufferAvg()
	{
		if (!m_pBufferAvg)
		{
			MAverageCalc::CreateInstance(&m_pBufferAvg);
			ATLASSERT(m_pBufferAvg);
			if (m_pBufferAvg)
				m_pBufferAvg->SetAverageCounter(16);
		}

		if (!m_pBufferAvg2)
		{
			MAverageCalc::CreateInstance(&m_pBufferAvg2);
			ATLASSERT(m_pBufferAvg2);
			if (m_pBufferAvg2)
				m_pBufferAvg2->SetAverageCounter(32);
		}

		return m_pBufferAvg && m_pBufferAvg2;
	}

	void _DestroyBufferAvg()
	{
		m_pBufferAvg.Release();
		m_pBufferAvg2.Release();
	}
};


#pragma warning(pop)
