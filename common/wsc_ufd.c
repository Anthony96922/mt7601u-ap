/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	wsc_ufd.c

	Abstract:

	Revision History:
	Who			When			What
*/

#include "rt_config.h"

#ifdef WSC_INCLUDED

static const char *XML_DECLARE_START = "<?xml";
static const char *XML_DECLARE_END = "?>";
static const char *XML_SSID_START = "<ssid";
static const char *XML_SSID_END = "</ssid>";
static const char *XML_AUTH_START = "<authentication>";
static const char *XML_AUTH_END = "</authentication>";
static const char *XML_ENCR_START = "<encryption>";
static const char *XML_ENCR_END = "</encryption>";
static const char *XML_KEY_START = "<networkKey";
static const char *XML_KEY_END = "</networkKey>";
static const char *XML_KEY_INDEX_START = "<keyIndex>";
static const char *XML_KEY_INDEX_END = "</keyIndex>";

static const char *XML_GUID_MARK = "CFG_GUID";
static const char *XML_AP_GUID_MARK = "CFG_AP_GUID";
static const char *XML_SSID_MARK = "CFG_SSID";
static const char *XML_AUTH_MARK = "CFG_AUTH";
static const char *XML_ENCR_MARK = "CFG_ENCR";
static const char *XML_KEY_MARK = "CFG_KEY";

static const char *XML_TEMPLATE =
"<?xml version=\"1.0\"?>\n"
"<wirelessProfile>\n"
"	<config>\n"
"		<configId>CFG_GUID"
"</configId>\n"
"		<configAuthorId>CFG_AP_GUID"
"</configAuthorId>\n"
"		<configAuthor>Ralink WPS AP</configAuthor>\n"
"	</config>\n"
"	<ssid xml:space=\"preserve\">CFG_SSID"
"</ssid>\n"
"	<connectionType>ESS</connectionType>\n"
"	<channel2Dot4>0</channel2Dot4>\n"
"	<channel5Dot0>0</channel5Dot0>\n"
"	<primaryProfile>\n"
"		<authentication>CFG_AUTH"
"</authentication>\n"
"		<encryption>CFG_ENCR"
"</encryption>\n"
"		<networkKey xml:space=\"preserve\">CFG_KEY"
"</networkKey>\n"
"		<keyProvidedAutomatically>0</keyProvidedAutomatically>\n"
"		<ieee802Dot1xEnabled>0</ieee802Dot1xEnabled>\n"
"	</primaryProfile>\n"
"</wirelessProfile>\n";


static struct {
	char * auth_str;
	unsigned short	auth_type;
} *PWSC_UFD_AUTH_TYPE, WSC_UFD_AUTH_TYPE[] = {
	{"open", 	WSC_AUTHTYPE_OPEN},
	{"shared",	WSC_AUTHTYPE_SHARED},
	{"WPA",		WSC_AUTHTYPE_WPA},
	{"WPA2",	WSC_AUTHTYPE_WPA2},
	{"WPAPSK",	WSC_AUTHTYPE_WPAPSK},
	{"WPA2PSK",	WSC_AUTHTYPE_WPA2PSK},
	{NULL,}
};


static struct {
	char * encr_str;
	unsigned short	encr_type;
} *PWSC_UFD_ENCR_TYPE, WSC_UFD_ENCR_TYPE[] = {
	{"none", 	WSC_ENCRTYPE_NONE},
	{"WEP",		WSC_ENCRTYPE_WEP},
	{"TKIP",	WSC_ENCRTYPE_TKIP},
	{"AES",		WSC_ENCRTYPE_AES},
	{NULL,}
};


bool	WscPassXmlDeclare(
	INOUT char **pXmlData)
{
	char	*ptr;

	DBGPRINT(RT_DEBUG_TRACE, ("======> WscPassXmlDeclare\n"));
	
	ptr = rtstrstr(*pXmlData, (char *)XML_DECLARE_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscPassXmlDeclare: missing XML Declare <?xml\n"));
		return FALSE;
	}

	ptr = rtstrstr(*pXmlData, (char *)XML_DECLARE_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("XML syntax error: missing XML Declare ?>\n"));
		return FALSE;
	}

	(*pXmlData) = ptr + strlen(XML_DECLARE_END);

	DBGPRINT(RT_DEBUG_TRACE, ("<====== WscPassXmlDeclare\n"));

	return TRUE;
}


bool WscGetXmlSSID(
	IN	char				*pXmlData,
	OUT	NDIS_802_11_SSID	*pSsid)
{
	char	*ptr, *pBuffer = pXmlData;

	ptr = rtstrstr(pBuffer, (char *)XML_SSID_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlSSID: missing <ssid\n"));
		return FALSE;
	}

	pBuffer = ptr + strlen(XML_SSID_START);
	
	ptr = rtstrstr(pBuffer, ">");
	if (ptr)
	{
		pBuffer = ptr + 1;
	}

	ptr = rtstrstr(pBuffer, (char *)XML_SSID_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlSSID: missing </ssid>\n"));
		return FALSE;
	}

	pSsid->SsidLength = (unsigned int)(ptr - pBuffer);
	RTMPZeroMemory(pSsid->Ssid, NDIS_802_11_LENGTH_SSID);
	if ((pSsid->SsidLength > 0) && (pSsid->SsidLength < 33))
	{
		RTMPMoveMemory(pSsid->Ssid, pBuffer, pSsid->SsidLength);
	}
	else
	{
		pSsid->SsidLength = 0;
		return FALSE;
	}

	return TRUE;
}


bool WscGetXmlAuth(
	IN	char	*pXmlData,
	OUT unsigned short	*pAuthType)
{
	char	*ptr, *pBuffer = pXmlData;
	char	AuthStr[10] = {0};
	unsigned int	AuthStrLen = 0;

	*pAuthType = 0;
	ptr = rtstrstr(pBuffer, (char *)XML_AUTH_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlAuth: missing %s\n", XML_AUTH_START));
		return FALSE;
	}

	pBuffer = ptr + strlen(XML_AUTH_START);

	ptr = rtstrstr(pBuffer, (char *)XML_AUTH_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlAuth: missing %s\n", XML_AUTH_END));
		return FALSE;
	}

	AuthStrLen = (unsigned int)(ptr - pBuffer);
	if ((AuthStrLen > 0) && (AuthStrLen <= 10))
	{
		RTMPMoveMemory(AuthStr, pBuffer, AuthStrLen);

		for (PWSC_UFD_AUTH_TYPE = WSC_UFD_AUTH_TYPE; PWSC_UFD_AUTH_TYPE->auth_str; PWSC_UFD_AUTH_TYPE++)
		{
			if (strcmp(AuthStr, PWSC_UFD_AUTH_TYPE->auth_str) == 0)
			{
				*pAuthType = PWSC_UFD_AUTH_TYPE->auth_type;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


bool WscGetXmlEncr(
	IN	char	*pXmlData,
	OUT unsigned short	*pEncrType)
{
	char	*ptr, *pBuffer = pXmlData;
	char	EncrStr[10] = {0};
	unsigned int	EncrStrLen = 0;

	*pEncrType = 0;
	ptr = rtstrstr(pBuffer, (char *)XML_ENCR_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlEncr: missing %s\n", XML_ENCR_START));
		return FALSE;
	}

	pBuffer = ptr + strlen(XML_ENCR_START);

	ptr = rtstrstr(pBuffer, (char *)XML_ENCR_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlEncr: missing %s\n", XML_ENCR_END));
		return FALSE;
	}

	EncrStrLen = (unsigned int)(ptr - pBuffer);
	if ((EncrStrLen > 0) && (EncrStrLen <= 10))
	{
		RTMPMoveMemory(EncrStr, pBuffer, EncrStrLen);

		for (PWSC_UFD_ENCR_TYPE = WSC_UFD_ENCR_TYPE; PWSC_UFD_ENCR_TYPE->encr_str; PWSC_UFD_ENCR_TYPE++)
		{
			if (strcmp(EncrStr, PWSC_UFD_ENCR_TYPE->encr_str) == 0)
			{
				*pEncrType = PWSC_UFD_ENCR_TYPE->encr_type;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


bool WscGetXmlKey(
	IN	char	*pXmlData,
	OUT unsigned char	*pKey,
	OUT unsigned short	*pKeyLen)
{
	char	*ptr, *pBuffer = pXmlData;
	unsigned int	KeyLen = 0;

	ptr = rtstrstr(pBuffer, (char *)XML_KEY_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlKey: missing %s\n", XML_KEY_START));
		return FALSE;
	}

	pBuffer = ptr + strlen(XML_KEY_START);
	
	ptr = rtstrstr(pBuffer, ">");
	if (ptr)
	{
		pBuffer = ptr + 1;
	}

	ptr = rtstrstr(pBuffer, (char *)XML_KEY_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlKey: missing %s\n", XML_KEY_END));
		return FALSE;
	}

	KeyLen = (unsigned int)(ptr - pBuffer);
	if ((KeyLen >= 8) && (KeyLen <= 64))
	{
		RTMPMoveMemory(pKey, pBuffer, KeyLen);
		*pKeyLen = KeyLen;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


bool WscGetXmlKeyIndex(
	IN	char	*pXmlData,
	OUT unsigned char	*pKeyIndex)
{
	char	*ptr, *pBuffer = pXmlData;

	*pKeyIndex = 1;
	ptr = rtstrstr(pBuffer, (char *)XML_KEY_INDEX_START);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlKeyIndex: missing %s\n", XML_KEY_INDEX_START));
		return FALSE;
	}

	pBuffer = ptr + strlen(XML_KEY_INDEX_START);

	ptr = rtstrstr(pBuffer, (char *)XML_KEY_INDEX_END);
	if (ptr == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("WscGetXmlKeyIndex: missing %s\n", XML_KEY_INDEX_END));
		return FALSE;
	}

	*pKeyIndex = *(--ptr) - 0x30;
	
	return TRUE;
}



bool	WscReadProfileFromUfdFile(
	IN RTMP_ADAPTER *pAd,
	IN unsigned char ApIdx,
	IN char * pUfdFileName)
{
	PWSC_CREDENTIAL     pCredential = &pAd->ApCfg.MBSSID[ApIdx].WscControl.WscProfile.Profile[0];
	RTMP_OS_FS_INFO		osFSInfo;
	struct file			*file_r;
	ssize_t				rv, fileLen = 0;
	char *				pXmlData = NULL;
	bool				RV = TRUE;

	if (pUfdFileName == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("--> %s: pUfdFileName is NULL\n", __FUNCTION__));
		return FALSE;
	}

	file_r = RtmpOSFileOpen(pUfdFileName, O_RDONLY, 0);
	if (IS_FILE_OPEN_ERR(file_r)) 
	{
		DBGPRINT(RT_DEBUG_TRACE, ("--> %s: Error opening file %s\n", __FUNCTION__, pUfdFileName));
		return FALSE;
	}
	else 
	{
		char tempStr[64] = {0};
		while((rv = RtmpOSFileRead(file_r, tempStr, 64)) > 0)
		{
			fileLen += rv;
		}
		os_alloc_mem(pAd, (unsigned char **)&pXmlData, fileLen+1);
		if (pXmlData == NULL)
		{
			RtmpOSFileClose(file_r);
			DBGPRINT(RT_DEBUG_TRACE, ("pXmlData kmalloc fail. (fileLen = %d)\n", fileLen));
			return FALSE;
		}
		RTMPZeroMemory(pXmlData, fileLen+1);
		RtmpOSFileSeek(file_r, 0);
		rv = RtmpOSFileRead(file_r, (char *)pXmlData, fileLen);
		RtmpOSFileClose(file_r);
		if (rv != fileLen)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("RtmpOSFileRead fail, fileLen = %d\n", fileLen));
			goto ReadErr;
		}
	}

	DBGPRINT(RT_DEBUG_TRACE, ("WscReadProfileFromUfdFile\n"));
	if (WscPassXmlDeclare(&pXmlData))
	{
		if (WscGetXmlSSID(pXmlData, &pCredential->SSID))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("SSID = %s(%d)\n", pCredential->SSID.Ssid, pCredential->SSID.SsidLength));
		}
		else
		{
			RV = FALSE;
			goto FreeXmlData;
		}

		if (WscGetXmlAuth(pXmlData, &pCredential->AuthType))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Credential.AuthType = 0x%04x\n", pCredential->AuthType));
		}
		else
		{
			RV = FALSE;
			goto FreeXmlData;
		}

		if (WscGetXmlEncr(pXmlData, &pCredential->EncrType))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Credential.EncrType = 0x%04x\n", pCredential->EncrType));
		}
		else
		{
			RV = FALSE;
			goto FreeXmlData;
		}

		pCredential->KeyLength = 0;
		RTMPZeroMemory(pCredential->Key, 64);
		if (WscGetXmlKey(pXmlData, pCredential->Key, &pCredential->KeyLength))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Credential.Key = %s (%d)\n", pCredential->Key, pCredential->KeyLength));
		}
		else
		{
			RV = FALSE;
			goto FreeXmlData;
		}

		/*
			If we cannot find keyIndex in .wfc file, use default value 1.
		*/
		if (WscGetXmlKeyIndex(pXmlData, &pCredential->KeyIndex))
		{
			DBGPRINT(RT_DEBUG_TRACE, ("pCredential->KeyIndex = %d\n", pCredential->KeyIndex));
		}
		
		DBGPRINT(RT_DEBUG_TRACE, ("WscReadProfileFromUfdFile OK\n"));

		WscWriteConfToPortCfg(pAd, 
							  &pAd->ApCfg.MBSSID[ApIdx].WscControl, 
							  &pAd->ApCfg.MBSSID[ApIdx].WscControl.WscProfile.Profile[0], TRUE);

		pAd->WriteWscCfgToDatFile = ApIdx;

		RtmpOsTaskWakeUp(&(pAd->wscTask));

FreeXmlData:
		if (pXmlData)
			os_free_mem(NULL, pXmlData);

		return RV;
	}

ReadErr:
	if (pXmlData)
		os_free_mem(NULL, pXmlData);
	return FALSE;
}


bool	WscWriteProfileToUfdFile(
	IN	PRTMP_ADAPTER		pAd,
	IN  unsigned char               ApIdx,
	IN  char *				pUfdFileName)
{
	PWSC_CTRL           pWscControl = &pAd->ApCfg.MBSSID[ApIdx].WscControl;
	PWSC_CREDENTIAL     pCredential = &pWscControl->WscProfile.Profile[0];
	RTMP_OS_FS_INFO		osFSInfo;
	struct file			*file_w;
	char *				offset, pXmlTemplate = (char *)XML_TEMPLATE;
	bool				bFound = FALSE, bRtn = TRUE;
	unsigned char				Guid[UUID_LEN_HEX];
	unsigned char				Guid_Str[UUID_LEN_STR];

	if (pUfdFileName == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("--> %s: pUfdFileName is NULL\n", __FUNCTION__));
		return FALSE;
	}

	file_w = RtmpOSFileOpen(pUfdFileName, O_WRONLY|O_TRUNC|O_CREAT, 0);
	if (IS_FILE_OPEN_ERR(file_w)) 
	{
		DBGPRINT(RT_DEBUG_TRACE, ("--> %s: Error opening file %s\n", __FUNCTION__, pUfdFileName));
		return FALSE;
	}
	else 
	{
		WscCreateProfileFromCfg(pAd, AP_MODE, pWscControl, &pWscControl->WscProfile);

		WscGenerateUUID(pAd, &Guid[0], &Guid_Str[0], ApIdx, TRUE);

		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_GUID_MARK)) != NULL)
		{
			RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));			
			RtmpOSFileWrite(file_w, (char *)&Guid_Str[0], (int)UUID_LEN_STR);
			pXmlTemplate = offset + strlen(XML_GUID_MARK);
		}
		
		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_AP_GUID_MARK)) != NULL)
		{
			RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));			
			RtmpOSFileWrite(file_w, (char *)&pWscControl->Wsc_Uuid_Str[0], (int)UUID_LEN_STR);
			pXmlTemplate = offset + strlen(XML_AP_GUID_MARK);
		}
		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_SSID_MARK)) != NULL)
		{
			RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));			
			RtmpOSFileWrite(file_w, (char *)&pCredential->SSID.Ssid[0], (int)pCredential->SSID.SsidLength);
			pXmlTemplate = offset + strlen(XML_SSID_MARK);
		}
		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_AUTH_MARK)) != NULL)
		{
			RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));			
			for (PWSC_UFD_AUTH_TYPE = WSC_UFD_AUTH_TYPE; PWSC_UFD_AUTH_TYPE->auth_str; PWSC_UFD_AUTH_TYPE++)
			{
				if (PWSC_UFD_AUTH_TYPE->auth_type == pCredential->AuthType)
				{
					RtmpOSFileWrite(file_w, 
									(char *)PWSC_UFD_AUTH_TYPE->auth_str, 
									(int)strlen(PWSC_UFD_AUTH_TYPE->auth_str));
					bFound = TRUE;
					break;
				}
			}
			pXmlTemplate = offset + strlen(XML_AUTH_MARK);
			if (bFound == FALSE)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("--> %s: Unknow Auth Type(=%x)\n", __FUNCTION__, pCredential->AuthType));
				bRtn = FALSE;
				goto out;
			}
		}
		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_ENCR_MARK)) != NULL)
		{
			bFound = FALSE;
			RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));			
			for (PWSC_UFD_ENCR_TYPE = WSC_UFD_ENCR_TYPE; PWSC_UFD_ENCR_TYPE->encr_str; PWSC_UFD_ENCR_TYPE++)
			{
				if (PWSC_UFD_ENCR_TYPE->encr_type == pCredential->EncrType)
				{
					RtmpOSFileWrite(file_w, 
									(char *)PWSC_UFD_ENCR_TYPE->encr_str, 
									(int)strlen(PWSC_UFD_ENCR_TYPE->encr_str));
					bFound = TRUE;
				}
			}
			pXmlTemplate = offset + strlen(XML_ENCR_MARK);
			if (bFound == FALSE)
			{
				DBGPRINT(RT_DEBUG_TRACE, ("--> %s: Unknow Encr Type(=%x)\n", __FUNCTION__, pCredential->EncrType));
				bRtn = FALSE;
				goto out;
			}
		}
		if ((offset = rtstrstr(pXmlTemplate, (char *)XML_KEY_MARK)) != NULL)
		{
			if (pCredential->EncrType != WSC_ENCRTYPE_NONE)
			{
				RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)(offset - pXmlTemplate));
				RtmpOSFileWrite(file_w,
								(char *)pCredential->Key,
								(int)pCredential->KeyLength);
				pXmlTemplate = offset + strlen(XML_KEY_MARK);
			}
			else
			{
				RtmpOSFileWrite(file_w, (char *)XML_ENCR_END, (int)strlen(XML_ENCR_END));
				RtmpOSFileWrite(file_w, (char *)"\n", (int)1);
				pXmlTemplate = offset + strlen(XML_KEY_MARK) + strlen(XML_KEY_END) + 1; /* 1: '\n' */
			}			
		}
		RtmpOSFileWrite(file_w, (char *)pXmlTemplate, (int)strlen(pXmlTemplate));
	}

out:
	RtmpOSFileClose(file_w);
	return bRtn;
}


#endif /* WSC_INCLUDED */
