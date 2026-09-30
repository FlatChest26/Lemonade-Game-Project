#pragma once

#include <iostream>

constexpr uint32_t SECONDS_IN_MINUTE = 60;
constexpr uint32_t MINUTES_IN_HOUR = 60;
constexpr uint32_t HOURS_IN_DAY = 24;
constexpr uint32_t MONTHS_IN_YEAR = 12;

struct TimeUnit
{
	uint32_t seconds;

	double total_hours() const { return total_minutes() / MINUTES_IN_HOUR; }
	double total_minutes() const { return total_seconds() / SECONDS_IN_MINUTE; }
	double total_seconds() const { return (double)seconds; }


	TimeUnit(double seconds = 0.0, double minutes = 0.0, double hours = 0.0):
		seconds(uint32_t(seconds + (minutes * SECONDS_IN_MINUTE) + (hours * MINUTES_IN_HOUR * SECONDS_IN_MINUTE)))
	{}

	void add(TimeUnit other)
	{
		seconds += other.seconds;
	}

	void print_time_12_hr()
	{
		uint32_t display_hours = (uint32_t)total_hours() % 12;
		uint32_t display_minutes = (uint32_t)total_minutes() % 60;

		if (display_hours == 0)
		{
			display_hours = 12;
		}

		std::cout << display_hours << ":" << display_minutes;

		if (total_hours() >= 12)
		{
			std::cout << " PM";
		}
		else
		{
			std::cout << " AM";
		}

		std::cout << std::endl;
	}

	// -- Operations -- //

	constexpr TimeUnit& operator=(const TimeUnit& other)
	{
		seconds = other.seconds;
		return *this;
	}

	template<typename U>
	constexpr TimeUnit& operator=(const U& other)
	{
		seconds = static_cast<U>(other.seconds);
		return *this;
	}

	// -- Comparisons -- //

	constexpr bool operator==(const TimeUnit& other) const
	{
		return seconds == other.seconds;
	}

	constexpr bool operator!=(const TimeUnit& other) const
	{
		return seconds != other.seconds;
	}

	constexpr bool operator>(const TimeUnit & other) const
	{
		return seconds > other.seconds;
	}

	constexpr bool operator>=(const TimeUnit& other) const
	{
		return seconds >= other.seconds;
	}

	constexpr bool operator<(const TimeUnit& other) const
	{
		return seconds < other.seconds;
	}

	constexpr bool operator<=(const TimeUnit& other) const
	{
		return seconds <= other.seconds;
	}
};

struct Month
{
	const char* name;
	const char* abbrv;
	uint32_t length;
};

inline const Month ALL_MONTHS[MONTHS_IN_YEAR] =
{
	{ "January",	"Jan",	31},
	{ "Feburary",	"Feb",	28},
	{ "March",		"Mar",	31},
	{ "April",		"Apr",	30},
	{ "May",		"May",	31},
	{ "June",		"Jun",	30},
	{ "July",		"Jul",	31},
	{ "August",		"Aug",	31},
	{ "September",	"Sep",	30},
	{ "October",	"Oct",	31},
	{ "November",	"Nov",	30},
	{ "December",	"Dec",	31},
};

inline constexpr uint32_t days_in_month( int month )
{
	return ALL_MONTHS[month].length;
}

struct TimeDate
{
	TimeUnit time_point;
	uint32_t day;
	uint32_t month;
	uint32_t year;

	void add( TimeUnit time_delta )
	{
		time_point.add( time_delta );
		fix_time();
	}

	void fix_time()
	{
		while ( (uint32_t) time_point.total_hours() >= HOURS_IN_DAY )
		{
			time_point.seconds -= (HOURS_IN_DAY * MINUTES_IN_HOUR * SECONDS_IN_MINUTE);
			day++;
		}

		while ( day >= days_in_month( month ) )
		{
			day -= days_in_month( month );
			month++;
		}

		while ( month >= MONTHS_IN_YEAR )
		{
			month -= MONTHS_IN_YEAR;
			year++;
		}
	}

	void print_date() const
	{
		std::cout << month << "/" << day << "/" << year << std::endl;
	}
};
