#pragma once

class ASVM
{
public:
	ASVM();
	~ASVM();
	
private:
	class Impl;
	Impl* pImpl;
};

