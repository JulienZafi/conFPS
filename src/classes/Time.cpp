#include "Time.hpp"

Time::Time()
{
	m_initTime = std::chrono::system_clock::now();
}

Time::~Time(){}

double Time::ElapsedTime() noexcept
{
	auto currentTime = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = currentTime - m_initTime;
	m_initTime = currentTime;
	double elapsedTime = duration.count();

	return elapsedTime;
}