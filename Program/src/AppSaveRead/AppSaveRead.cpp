#include "AppSaveRead.h"

int ReadWrite::ReadInteger(const char* m_szFileName, const char* szSection, const char* szKey, int iDefaultValue)
{
    int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
    return iResult;
}

float ReadWrite::ReadFloat(const char* m_szFileName, const char* szSection, const char* szKey, float fltDefaultValue)
{
    char szResult[255];
    char szDefault[255];
    float fltResult;
    sprintf(szDefault, "%f", fltDefaultValue);
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    fltResult = (float)atof(szResult);
    return fltResult;
}

bool ReadWrite::ReadBoolean(const char* m_szFileName, const char* szSection, const char* szKey, bool bolDefaultValue)
{
    char szResult[255];
    char szDefault[255];
    bool bolResult;
    sprintf(szDefault, "%s", bolDefaultValue ? "True" : "False");
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    bolResult = (strcmp(szResult, "True") == 0 ||
        strcmp(szResult, "true") == 0) ? true : false;
    return bolResult;
}

char* ReadWrite::ReadString(const char* m_szFileName, const char* szSection, const char* szKey, const char* szDefaultValue)
{
    char* szResult = new char[255];
    memset(szResult, 0x00, 255);
    GetPrivateProfileString(szSection, szKey,
        szDefaultValue, szResult, 255, m_szFileName);
    return szResult;
}

void ReadWrite::WriteInteger(const char* m_szFileName, const char* szSection, const char* szKey, int iValue)
{
    char szValue[255];
    sprintf(szValue, "%d", iValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void ReadWrite::WriteFloat(const char* m_szFileName, const char* szSection, const char* szKey, float fltValue)
{
    char szValue[255];
    sprintf(szValue, "%f", fltValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void ReadWrite::WriteBoolean(const char* m_szFileName, const char* szSection, const char* szKey, bool bolValue)
{
    char szValue[255];
    sprintf(szValue, "%s", bolValue ? "True" : "False");
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

void ReadWrite::WriteString(const char* m_szFileName, const char* szSection, const char* szKey, const char* szValue)
{
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}