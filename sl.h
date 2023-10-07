#ifndef OHOOKSL_H
#define OHOOKSL_H

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

#endif
