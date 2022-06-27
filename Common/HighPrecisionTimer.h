#pragma once

#include <chrono>
#include <ratio>

#include <Common/Kernel.h>

namespace SK
{
	class HighPrecisionTimer
	{
	public:
		std::chrono::time_point< std::chrono::steady_clock >	GetStart();
		UInt64		GetNanosecondsTime();
		UInt64		GetMicrosecondTime();
		UInt64		GetMillisecondsTime();
		UInt64		GetSecondsTime();
		UInt64		GetMinutesTime();
		UInt64		GetHoursTime();
		UInt64		GetDaysTime();
		UInt64		GetWeeksTime();
		void		Reset();
		void		Stop();

	private:
		typedef	std::chrono::duration< UInt64, std::ratio< 3600*24 > >			Days;
		typedef	std::chrono::duration< UInt64, std::ratio< 3600*24*7 > >		Weeks;
		typedef	std::chrono::duration< UInt64, std::ratio< 3600*24*7*365 > >	Years;

		std::chrono::time_point< std::chrono::steady_clock > m_oStart;
		std::chrono::time_point< std::chrono::steady_clock > m_oEnd;
	};
}
