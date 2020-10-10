#pragma once
class IoPeerBase;
class ReactorBase
{
public:
	ReactorBase();
	virtual ~ReactorBase();
	virtual void AddIoPeer(IoPeerBase *pIoPeer);
	virtual	void Start(UINT32 nThread);
	virtual void Stop();

};

