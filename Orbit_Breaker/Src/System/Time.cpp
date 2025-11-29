#include "Time.hpp"

#include <chrono>
#include <thread>

Time::Time() :
	delta_time{0.f}, limit_frame_rate{}, ideal_delta_time{0.02f},
	worst_ideal_delta_time{0.05f}, frame_amount{0}, frame_rate{0},
	frame_time{0.f}, delay_time{0.0f}
{
}

void Time::Init(const bool limit_frame_rate, const double ideal_delta_time,
	const double worst_ideal_delta_time)
{
	this->limit_frame_rate = limit_frame_rate;
	this->ideal_delta_time = ideal_delta_time;
	this->worst_ideal_delta_time = worst_ideal_delta_time;
}

void Time::StartTimer()
{
	current_time = std::chrono::steady_clock::now();
	previous_time = current_time;
}

void Time::Update()
{
	// Calculate delta time since last frame
	delta_time = CalculateDeltaTime();

	if (delta_time > worst_ideal_delta_time)
	{
		delta_time = worst_ideal_delta_time;
	}

	if (limit_frame_rate)
	{
		// Schedule the next frame based on previous frame start and
		// `ideal_delta_time`
		auto target_time = previous_time +
			std::chrono::duration_cast<std::chrono::steady_clock::duration>(
				std::chrono::duration<double>(ideal_delta_time));
		std::this_thread::sleep_until(target_time);

		// Lock `delta_time` to ideal frame duration
		delta_time = ideal_delta_time;
		previous_time = target_time;
	}

	frame_time += delta_time;
	frame_amount++;

	// Calculate frame rate
	if (frame_time >= 1.f)
	{
		frame_rate = frame_amount;
		frame_amount = 0;
		frame_time -= 1.f;
	}
}

int Time::GetFrameRate() const
{
	return frame_rate;
}

float Time::GetDeltaTime() const
{
	return static_cast<float>(delta_time);
}

double Time::CalculateDeltaTime()
{
	previous_time = current_time;
	current_time = std::chrono::steady_clock::now();

	using dt_seconds = std::chrono::duration<double>;
	return std::chrono::duration_cast<dt_seconds>(
		current_time - previous_time).count();
}
