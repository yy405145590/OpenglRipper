#pragma once
#include <windows.h>
#include <map>
typedef void* InstanceKey;
class LockSection
{
public:
	LockSection();
	~LockSection();

	void Lock();
	void Unlock();
private:
	CRITICAL_SECTION m_section;
};
extern InstanceKey GetID();
template <typename T>
class TlsInstance
{
public:	
	static T* GetInstance();
protected:
	TlsInstance(void){}
	~TlsInstance(void){}

private:
	static std::map<InstanceKey, T*> sMapTlsInstance;
	static LockSection			sLock;
};
template<typename T>
T* TlsInstance<T>::GetInstance()
{
	sLock.Lock();
	InstanceKey tId = GetID();
	if (sMapTlsInstance[tId] == NULL)
	{
		sMapTlsInstance[tId] = new T();
	}
	T* pInstance = sMapTlsInstance[tId];
	sLock.Unlock();
	return pInstance;
}

template<typename T>
LockSection TlsInstance<T>::sLock;

template<typename T>
std::map<InstanceKey, T*> TlsInstance<T>::sMapTlsInstance;

