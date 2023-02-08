#pragma once

template<typename T>
class Singleton
{
public:
	//Singleton() = delete;
	Singleton(void){}
	Singleton(const Singleton& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;
	//virtual ~Singleton() = default;

	static T* GetInstance()  {
		static T instance;
		return &instance;
	}

};

