#pragma once
#include "AppIncludes.h"

class ReadWrite
{
public:
	int ReadInteger(const char* m_szFileName, const char* szSection, const char* szKey, int iDefaultValue);
	float ReadFloat(const char* m_szFileName, const char* szSection, const char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* m_szFileName, const char* szSection, const char* szKey, bool bolDefaultValue);
	char* ReadString(const char* m_szFileName, const char* szSection, const char* szKey, const char* szDefaultValue);

	void WriteInteger(const char* m_szFileName, const char* szSection, const char* szKey, int iValue);
	void WriteFloat(const char* m_szFileName, const char* szSection, const char* szKey, float fltValue);
	void WriteBoolean(const char* m_szFileName, const char* szSection, const char* szKey, bool bolValue);
	void WriteString(const char* m_szFileName, const char* szSection, const char* szKey, const char* szValue);
};