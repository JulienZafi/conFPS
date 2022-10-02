#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

class Time
{
public:
	Time();
	~Time();

	double ElapsedTime() noexcept;

private:
	std::chrono::system_clock::time_point m_initTime;
};

#endif // !TIME_HPP