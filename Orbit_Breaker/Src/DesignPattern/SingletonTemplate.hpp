#pragma once

template <typename T>
class SingletonTemplate
{
public:
	static T& Get()
	{
		static T instance;

		return instance;
	}

	SingletonTemplate(const SingletonTemplate&) = delete;
	SingletonTemplate& operator=(const SingletonTemplate&) = delete;
	SingletonTemplate(SingletonTemplate&&) = delete;
	SingletonTemplate& operator=(SingletonTemplate&&) = delete;

protected:
	SingletonTemplate() = default;
	~SingletonTemplate() = default;
};