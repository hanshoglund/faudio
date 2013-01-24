
#ifndef _DOREMIR_SET
#define _DOREMIR_SET

#include <doremir.h>
#include <doremir/list.h>
#include <doremir/string.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirSet Set
    @{
    */

typedef struct _doremir_set_t * doremir_set_t;
doremir_set_t doremir_set_empty();
doremir_set_t doremir_set_single(doremir_ptr_t);
doremir_set_t doremir_set_add(doremir_ptr_t, doremir_set_t);
doremir_set_t doremir_set_remove(doremir_ptr_t, doremir_set_t);
doremir_set_t doremir_set_dadd(doremir_ptr_t, doremir_set_t);
doremir_set_t doremir_set_dremove(doremir_ptr_t, doremir_set_t);
doremir_set_t doremir_set_copy(doremir_set_t);
void doremir_set_destroy(doremir_set_t);
int doremir_set_size(doremir_set_t);
bool doremir_set_is_empty(doremir_set_t);
bool doremir_set_is_single(doremir_set_t);
bool doremir_set_has(doremir_ptr_t, doremir_set_t);
bool doremir_set_is_subset_of(doremir_set_t, doremir_set_t);
bool doremir_set_is_proper_subset_of(doremir_set_t, doremir_set_t);
doremir_set_t doremir_set_sum(doremir_set_t, doremir_set_t);
doremir_set_t doremir_set_product(doremir_set_t, doremir_set_t);
doremir_set_t doremir_set_difference(doremir_set_t, doremir_set_t);
doremir_set_t doremir_set_power(doremir_set_t);
doremir_set_t doremir_set_from_list(doremir_list_t);
doremir_list_t doremir_set_to_list(doremir_set_t);

/** @}
    @}
    */

#endif // _DOREMIR_SET

