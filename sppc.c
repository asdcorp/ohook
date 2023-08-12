#define _WIN32_WINNT _WIN32_WINNT_WIN10
#include <windows.h>
#include <shlwapi.h>

typedef GUID SLID;
typedef void *HSLC;

typedef struct {
    SLID SkuId;
    DWORD eStatus;
    DWORD dwGraceTime;
    DWORD dwTotalGraceDays;
    HRESULT hrReason;
    UINT64 qwValidityExpiration;
} SL_LICENSING_STATUS;

typedef enum {
    SL_DATA_NONE = REG_NONE,
    SL_DATA_SZ = REG_SZ,
    SL_DATA_DWORD = REG_DWORD,
    SL_DATA_BINARY = REG_BINARY,
    SL_DATA_MULTI_SZ,
    SL_DATA_SUM = 100
} SLDATATYPE;

HRESULT WINAPI SLGetLicensingStatusInformation(
    HSLC hSLC,
    SLID *pAppID,
    SLID *pProductSkuId,
    PWSTR pwszRightName,
    UINT *pnStatusCount,
    SL_LICENSING_STATUS **ppLicensingStatus
);

HRESULT WINAPI SLGetProductSkuInformation(
    HSLC hSLC,
    const SLID *pProductSkuId,
    PCWSTR pwszValueName,
    SLDATATYPE *peDataType,
    UINT *pcbValue,
    PBYTE *ppbValue
);

BOOL APIENTRY WINAPI dll_main(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
) {
    return TRUE;
}

BOOL check_for_grace(HSLC hSLC, SLID *pProductSkuId) {
    PBYTE pBuffer = 0;
    UINT cbSize = 0;

    if(SLGetProductSkuInformation(hSLC, pProductSkuId, L"Name", NULL, &cbSize, &pBuffer) != S_OK) {
        LocalFree(pBuffer);
        return FALSE;
    }

    if(StrStrNIW((PWSTR)pBuffer, L"Grace", cbSize) != NULL) {
        LocalFree(pBuffer);
        return TRUE;
    }

    LocalFree(pBuffer);
    return FALSE;
}

HRESULT WINAPI SLGetLicensingStatusInformationHook(
    HSLC hSLC,
    SLID *pAppID,
    SLID *pProductSkuId,
    PWSTR pwszRightName,
    UINT *pnStatusCount,
    SL_LICENSING_STATUS **ppLicensingStatus
) {
    HRESULT hResult = SLGetLicensingStatusInformation(
        hSLC,
        pAppID,
        pProductSkuId,
        pwszRightName,
        pnStatusCount,
        ppLicensingStatus
    );

    if(hResult != S_OK)
        return hResult;

    for(int i = 0; i < *pnStatusCount; i++) {
        if((*ppLicensingStatus+i)->eStatus == 0) continue;
        if(check_for_grace(hSLC, &((*ppLicensingStatus+i)->SkuId))) continue;

        (*ppLicensingStatus+i)->eStatus = 1;
        (*ppLicensingStatus+i)->dwGraceTime = 0;
        (*ppLicensingStatus+i)->dwTotalGraceDays = 0;
        (*ppLicensingStatus+i)->hrReason = 0;
        (*ppLicensingStatus+i)->qwValidityExpiration = 0;
    }

    return hResult;
}
