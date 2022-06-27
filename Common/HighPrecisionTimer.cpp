#include <Common/HighPrecisionTimer.h>

#include <chrono>

namespace SK
{
	std::chrono::time_point< std::chrono::steady_clock >	HighPrecisionTimer::GetStart()
	{
		return m_oStart;
	}

	UInt64	HighPrecisionTimer::GetNanosecondsTime()
	{
		return std::chrono::duration_cast< std::chrono::nanoseconds >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetMicrosecondTime()
	{
		return std::chrono::duration_cast< std::chrono::microseconds >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetMillisecondsTime()
	{
		return std::chrono::duration_cast< std::chrono::milliseconds >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetSecondsTime()
	{
		return std::chrono::duration_cast< std::chrono::seconds >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetMinutesTime()
	{
		return std::chrono::duration_cast< std::chrono::minutes >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetHoursTime()
	{
		return std::chrono::duration_cast< std::chrono::hours >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetDaysTime()
	{
		return std::chrono::duration_cast< HighPrecisionTimer::Days >( m_oEnd - m_oStart ).count();
	}

	UInt64	HighPrecisionTimer::GetWeeksTime()
	{
		return std::chrono::duration_cast< HighPrecisionTimer::Weeks >( m_oEnd - m_oStart ).count();
	}

	void	HighPrecisionTimer::Reset()
	{
		m_oStart = std::chrono::high_resolution_clock::now();
	}

	void	HighPrecisionTimer::Stop()
	{
		m_oEnd = std::chrono::high_resolution_clock::now();
	}
}
