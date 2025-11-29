#pragma once

#include "../DesignPattern/SingletonTemplate.hpp"

#include <chrono>

class Time : public SingletonTemplate<Time>
{
	friend SingletonTemplate<Time>;

public:
	void Init(const bool limit_frame_rate = true,
		const double ideal_delta_time = 1.f / 60.f,
		const double worst_ideal_delta_time = 0.05f);
	void StartTimer();
	void Update();

	int GetFrameRate() const;
	float GetDeltaTime() const;

protected:
	Time();

private:
	double CalculateDeltaTime();

	std::chrono::steady_clock::time_point current_time;
	std::chrono::steady_clock::time_point previous_time;

	double delta_time;
	bool limit_frame_rate;
	double ideal_delta_time;
	double worst_ideal_delta_time;
	int frame_amount;
	int frame_rate;
	double frame_time;
	double delay_time;
};
