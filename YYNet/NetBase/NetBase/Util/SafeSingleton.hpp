#pragma once

template <typename T>
class SafeSingleton
{
public:
	static T * SafeSingleton::getInstance()
	{
		T* tmp = m_instance.load();
		if (tmp == nullptr) 
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			tmp = m_instance.load();
			if (tmp == nullptr) 
			{
				tmp = new T();
				m_instance.store(tmp);
			}
		}
		return tmp;
	}
protected:
	// 禁止直接创建Singleton对象
	SafeSingleton() 
	{		
	}
	virtual ~SafeSingleton()
	{		
	}
private:
	SafeSingleton(const SafeSingleton& source) {}
	
	static std::atomic<T*> m_instance;
	static std::mutex m_mutex;
};
template <typename T>
std::atomic<T*> SafeSingleton<T>::m_instance;

template <typename T>
std::mutex SafeSingleton<T>::m_mutex;