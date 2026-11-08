#ifndef PGXP_DEFS_H
#define PGXP_DEFS_H

#include "PsyX/PsyX_config.h"

#if USE_PGXP
#include "PsyX/common/half_float.h"

// Helpful macro
#define PGXP_LOOKUP_VALUE(x, y) (*(u_short*)&(x) | (*(u_short*)&(y) << 16))

//-------------------------------------

// in C++, VERTTYPE can be declared as half
// use `_HF` to convert to half whenever you in C mode.
#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
typedef half VERTTYPE;
#define _HF(x) x
#else
typedef short VERTTYPE;
#define _HF(x) to_half_float(x)
#endif

typedef struct
{
	float px, py, pz;	// 32 bit values
	VERTTYPE x, y, z;	// 16 bit values (for lookup and backwards compat if not found in cache)
} PGXPVector3D;

typedef struct
{
	uint lookup;
	float px, py, pz;
	float scr_h, ofx, ofy;
} PGXPVData;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern PGXPVector3D g_FP_SXYZ0; // direct access PGXP without table lookup
extern PGXPVector3D g_FP_SXYZ1;
extern PGXPVector3D g_FP_SXYZ2;
#define PGXP_INVALID_INDEX	0xFFFFFF	// [mod] 24-bit vertex index (was 0xFFFF)
#define PGXP_CACHE_SIZE		(1 << 20)

extern uint g_FP_SXYZ0Index;
extern uint g_FP_SXYZ1Index;
extern uint g_FP_SXYZ2Index;

/* clears PGXP vertex buffer */
void	PGXP_ClearCache();

/* emits new PGXP vertex */
uint	PGXP_EmitCacheData(PGXPVData* newData);

/* sets Z offset (works like Z bias) */
void	PGXP_SetZOffsetScale(float offset, float scale);

/* searches for vertex with given lookup value */
int		PGXP_GetCacheData(PGXPVData* out, uint lookup, uint indexhint /* = PGXP_INVALID_INDEX */);
int		PGXP_GetCacheDataExact(PGXPVData* out, uint lookup, uint indexhint);

/* used by primitive setup */
uint	PGXP_GetIndex(int checkTransform);
void	PGXP_RecordAddressIndex(const void* address, uint index);
uint	PGXP_GetAddressIndex(const void* address);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

// special PGXP type
typedef struct {		/* 2D short vector */
	half vx, vy;
	ushort pgxp_index;
} DVECTORF;

#else
typedef short VERTTYPE;

#define _HF(x) x

#endif


#endif // PGXP_DEFS
