#pragma once

#include <format>
#include <iostream>
#include <string>

class Echo
{
public:
	enum class EchoLevel
	{
		Debug,
		Info,
		Warn,
		Error,
	};

	template <typename... Args>
	static void Debug(std::string_view fmt, Args&&... args)
	{
		Log(EchoLevel::Debug, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Info(std::string_view fmt, Args&&... args)
	{
		Log(EchoLevel::Info, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Warn(std::string_view fmt, Args&&... args)
	{
		Log(EchoLevel::Warn, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Error(std::string_view fmt, Args&&... args)
	{
		Log(EchoLevel::Error, fmt, std::forward<Args>(args)...);
	}

private:
	static constexpr std::string_view RESET = "\033[0m";

	template <typename... Args>
	static void Log(EchoLevel level, std::string_view fmt, Args&&... args)
	{
		std::string color;
		std::string levelStr;
		switch (level)
		{
		case EchoLevel::Debug:
			// Cyan
			color = "\033[36m";
			levelStr = "DEBUG";
			break;

		case EchoLevel::Info:
			levelStr = "INFO";
			break;

		case EchoLevel::Warn:
			// Yellow
			color = "\033[33m";
			levelStr = "WARN";
			break;

		case EchoLevel::Error:
			// Red
			color = "\033[31m";
			levelStr = "ERROR";
			break;
		}

		std::string message = std::vformat(fmt, std::make_format_args(args...));
		std::cout << std::format("{}[{}]: {}{}\n", color, levelStr, message,
			RESET);
	}
};
