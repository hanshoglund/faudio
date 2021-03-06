
#ifndef _FA_LIST
#define _FA_LIST

#include <fa.h>

/** @addtogroup FaList

    Immutable list data structure.

    @par Literals
    - `list(1, 2, 3)`

    @par Display
    - `[1,2,3]`
    
    @par Iteration
    ~~~
    fa_for_each (x, list(1,2,3))
    {
        fa_print("%s\n", x);
    }
    ~~~

    @par Implements
    - fa_equal_t
    - fa_order_t
    - fa_copy_t
    - fa_destroy_t
    - fa_dynamic_t
    - fa_string_show_t

    @see
    - [Data structures](@ref DataStructures)

 
    @defgroup Fa Fa
    @{
    @defgroup FaList List
    @{
    */


typedef struct _fa_list_t * fa_list_t;

/** A sort predicate function. Should return true if a < b
*/
typedef bool (* fa_list_sort_fn_t)(fa_ptr_t a, fa_ptr_t b);

/** Create an empty list.

    The returned list must be destroyed by passing to a destructive function.

    @return
        A new list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_empty();

/** Create a new list containing the given element.

    The returned list must be destroyed by passing to a destructive function.

    @param value
        Value to store.
    @return
        A new list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_single(fa_ptr_t ptr);

/** Create a new list by inserting the given element at the beginning of the given list.

    The returned list must be destroyed by passing to a destructive function.

    @param head
        Value to form head of list.
    @param tail
        List to form tail of list.
    @return
        A new list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_cons(fa_ptr_t ptr, fa_list_t list);

/** Create a new list by inserting the given element at the beginning of the given list.

    The returned list must be destroyed by passing to a destructive function.

    @param head
        Value to form head of list.
    @param tail
        List to form tail of list.
    @return
        A new list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_dcons(fa_ptr_t ptr, fa_list_t list);

/** Create a list by repeating the given element.
*/
fa_list_t fa_list_repeat(int int_, fa_ptr_t ptr);

/** Create a list from the given range.
*/
fa_list_t fa_list_enumerate(int int_, int int__);

/** Copy the given list.

    The returned list must be destroyed by passing to a destructive function.

    @par Performance
        O(1)
*/
fa_list_t fa_list_copy(fa_list_t list);

/** Copy the given list and all contained elements recursivly.

      @par Performance
    O(n)
    
*/
fa_list_t fa_list_deep_copy(fa_list_t list);

/** Destroy the given list.

    @par Performance
        O(n)
*/
void fa_list_destroy(fa_list_t list);

/** Destroy the given list and all contained elements.

    @par Performance
        O(n)
    
*/
void fa_list_deep_destroy(fa_list_t list,
                          fa_deep_destroy_pred_t deepDestroyPred);

/** Return whether the given list is empty.

    @par Performance
        O(1)
*/
bool fa_list_is_empty(fa_list_t list);

/** Return whether the given list has a single element.
    @par Performance
        O(1)
*/
bool fa_list_is_single(fa_list_t list);

/** Return the lenght of the given list.
    @par Performance
        O(n)
*/
int fa_list_length(fa_list_t list);

/** Return the first element of the given list.
    @par Performance
        O(1)
*/
fa_ptr_t fa_list_head(fa_list_t list);

/** Return all elements but the first of the given list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_tail(fa_list_t list);

/** Return all elements but the first of the given list.
    @par Performance
        O(1)
*/
fa_list_t fa_list_dtail(fa_list_t list);

/** Return all elements but the last of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_init(fa_list_t list);

/** Return all elements but the last of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dinit(fa_list_t list);

/** Return the last element of the given list.
    @par Performance
        O(n)
*/
fa_ptr_t fa_list_last(fa_list_t list);

/** Return the result of appending the given lists.
    @par Performance
        O(n)
*/
fa_list_t fa_list_append(fa_list_t list, fa_list_t list_);

/** Return the result of appending the given lists.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dappend(fa_list_t list, fa_list_t list_);

/** Return the reverse of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_reverse(fa_list_t list);

/** Return the reverse of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dreverse(fa_list_t list);

/** Return a sorted version of the given list, according to the given sort function.
    The passed list is unchanged.
    @par Performance
        O(n log n)
*/
fa_list_t fa_list_sort(fa_list_t list, fa_list_sort_fn_t sortFn);

/** Sort the given list in place, according to the given sort function. Returns the list.
    @par Performance
        O(n log n)
*/
fa_list_t fa_list_dsort(fa_list_t list, fa_list_sort_fn_t sortFn);

/** Sort the given list ascending in place. Returns the list.
    fa_list_sort_ascending(x) == fa_list_sort(x, fa_less_than)
    
    @par Performance
        O(n log n)
    
*/
fa_list_t fa_list_dsort_ascending(fa_list_t list);

/** Return the *n* leading elements of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_take(int int_, fa_list_t list);

/** Return the *n* leading elements of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dtake(int int_, fa_list_t list);

/** Return the all but the *n* leading elements of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_drop(int int_, fa_list_t list);

/** Return the all but the *n* leading elements of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_ddrop(int int_, fa_list_t list);

/** List index operator.
    @returns
        The nth element of the given list.
    @par Performance
        O(n)
*/
fa_ptr_t fa_list_index(int int_, fa_list_t list);

/** Return the given range of the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_range(int int_, int int__, fa_list_t list);

/** Insert the given element into the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_insert(int int_, fa_ptr_t ptr, fa_list_t list);

/** Insert the given element into the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dinsert(int int_, fa_ptr_t ptr, fa_list_t list);

/** Insert the given range into the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_insert_range(int int_,
                               fa_list_t list,
                               fa_list_t list_);

/** Insert the given range into the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dinsert_range(int int_,
                                fa_list_t list,
                                fa_list_t list_);

/** Remove the given element from the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_remove(int int_, fa_list_t list);

/** Remove the given element from the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dremove(int int_, fa_list_t list);

/** Remove the given range from the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_remove_range(int int_, int int__, fa_list_t list);

/** Remove the given range from the given list.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dremove_range(int int_,
                                int int__,
                                fa_list_t list);

/** Return whether the given list contains the given element.
    @par Performance
        O(n)
*/
bool fa_list_has(fa_ptr_t ptr, fa_list_t list);

/** Return the first element satisfying the given predicate in the
    list, if found.
    @param list     List.
    @param value    Value to search for.
    @return         Index of the found value (optional).
    @par Performance
        O(log n)
*/
fa_ptr_t fa_list_find(fa_pred_t pred, fa_ptr_t ptr, fa_list_t list);

/** Return the index of the first occurance given element in the
    list, or a negative value if no such element is found
    @par Performance
        O(n)
*/
int fa_list_index_of(fa_ptr_t ptr, fa_list_t list);

/** Return the index of the first element satisfying the given predicate in the
    list, or a negative value if no such element is found.
    @par Performance
        O(log n)
*/
int fa_list_find_index(fa_pred_t predicate,
                       fa_ptr_t predicateData,
                       fa_list_t input);

/** Return a list that is the same as the given list with duplicates removed.
    @par Performance
        O(n^2)
*/
fa_list_t fa_list_remove_duplicates(fa_list_t list);

/** Create a list containing the paired contents of the given lists.

    @par Example
        
        fa_list_zip(fa_list(1,2,3), fa_list(4,5,6)) == fa_list(fa_pair_create(1,4), fa_pair_create(2,5), fa_pair_create(3,6))

    @par Performance
        O(n)
*/
fa_list_t fa_list_zip(fa_list_t list, fa_list_t list_);

/** Return the given list with all elements not satisfying the given predicate removed.
    @par Performance
        O(n)
*/
fa_list_t fa_list_filter(fa_pred_t predicate,
                         fa_ptr_t predicateData,
                         fa_list_t input);

/** Return the given list with all elements not satisfying the given predicate removed.
    @par Performance
        O(n)
*/
fa_list_t fa_list_dfilter(fa_pred_t predicate,
                          fa_ptr_t predicateData,
                          fa_list_t input);

/** Return the result of applying the given function to all elements of the given list.

    @par Laws

        map(apply1, id, xs)                == xs
        map(apply1, f, map(apply1, g, xs)) == map(apply1, comp(f, g), xs)

    @par Performance
        O(n)
*/
fa_list_t fa_list_map(fa_unary_t function,
                      fa_ptr_t functionData,
                      fa_list_t input);

/** Return the result of applying the given function to all elements of the given list.

    @par Laws

        map(apply1, id, xs)                == xs
        map(apply1, f, map(apply1, g, xs)) == map(apply1, comp(f, g), xs)

    @par Performance
        O(n)
*/
fa_list_t fa_list_dmap(fa_unary_t function,
                       fa_ptr_t functionData,
                       fa_list_t input);

/**
     Map over the given list and join the results.
     
    This function is useful to apply functions from singletons to lists.
     
    @par Laws
     
        joinMap(apply1, single, xs) == xs`
     
    @par Performance
        O(n)
     
*/
fa_list_t fa_list_join_map(fa_unary_t unary,
                           fa_ptr_t ptr,
                           fa_list_t list);


fa_list_t fa_list_djoin_map(fa_unary_t unary,
                            fa_ptr_t ptr,
                            fa_list_t list);

/** Concatenate all elements of the given list.

    The given list must contain lists only.

    @par Performance
        O(n)
*/
fa_list_t fa_list_join(fa_list_t list);

/** Concatenate all elements of the given list.

    The given list must contain lists only.

    @par Performance
        O(n)
*/
fa_list_t fa_list_djoin(fa_list_t list);

/** Fold over the given list from left to right.

    @par Performance
        O(n)
*/
fa_ptr_t fa_list_fold_left(fa_binary_t binary,
                           fa_ptr_t ptr,
                           fa_ptr_t ptr_,
                           fa_list_t list);

/** Fold over the given list from left to right.

    @par Performance
        O(n)
*/
fa_ptr_t fa_list_dfold_left(fa_binary_t binary,
                            fa_ptr_t ptr,
                            fa_ptr_t ptr_,
                            fa_list_t list);


fa_list_t fa_list_to_list(fa_list_t list);


void fa_list_log_count();

/** @}
    @}
    */

#endif // _FA_LIST

