
#ifndef _DOREMIR_RATIO
#define _DOREMIR_RATIO

#include <doremir/std.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirRatio Ratio
    @{
    */

typedef int32_t doremir_ratio_nom_t;
typedef uint32_t doremir_ratio_denom_t;
typedef struct {
            doremir_ratio_nom_t nom; doremir_ratio_denom_t denom;
        } doremir_ratio_t;
doremir_ratio_t doremir_ratio_add(doremir_ratio_t,
                                  doremir_ratio_t);
doremir_ratio_t doremir_ratio_subtract(doremir_ratio_t,
                                       doremir_ratio_t);
doremir_ratio_t doremir_ratio_multiply(doremir_ratio_t,
                                       doremir_ratio_t);
doremir_ratio_t doremir_ratio_divide(doremir_ratio_t,
                                     doremir_ratio_t);
doremir_ratio_t doremir_ratio_remainder(doremir_ratio_t,
                                        doremir_ratio_t);
doremir_ratio_t doremir_ratio_succ(doremir_ratio_t);
doremir_ratio_t doremir_ratio_pred(doremir_ratio_t);
doremir_ratio_t doremir_ratio_negate(doremir_ratio_t);
doremir_ratio_t doremir_ratio_recip(doremir_ratio_t);

/** @}
    @}
    */

#endif // _DOREMIR_RATIO

