#define _WIN32_WINNT _WIN32_WINNT_WIN10
#include <windows.h>

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

HRESULT WINAPI SLGetLicensingStatusInformation(
    HSLC hSLC,
    SLID *pAppID,
    SLID *pProductSkuId,
    PWSTR pwszRightName,
    UINT *pnStatusCount,
    SL_LICENSING_STATUS **ppLicensingStatus
);

BOOL APIENTRY WINAPI dll_main(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
) {
    return TRUE;
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

        (*ppLicensingStatus+i)->eStatus = 1;
        (*ppLicensingStatus+i)->dwGraceTime = 0;
        (*ppLicensingStatus+i)->dwTotalGraceDays = 0;
        (*ppLicensingStatus+i)->hrReason = 0;
        (*ppLicensingStatus+i)->qwValidityExpiration = 0;
    }

    return hResult;
}
