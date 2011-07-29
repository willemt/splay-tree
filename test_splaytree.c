/*
 * =====================================================================================
 *
 *       Filename:  test_splaytree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/26/11 22:19:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

#include "splaytree.h"


static int __mem_compare(
    const void *e1,
    const void *e2
)
{
    return e1 - e2;
}

void TestSplaytree_init_is_empty(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    CuAssertTrue(tc, splaytree_is_empty(st));
    CuAssertTrue(tc, 0 == splaytree_count(st));
    splaytree_free(st);
}

void TestSplaytree_init_means_peek_is_empty(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    CuAssertTrue(tc, NULL == splaytree_peek(st));
    splaytree_free(st);
}

void TestSplaytree_post_put_means_peek_exists(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 10);
    CuAssertTrue(tc, NULL != splaytree_peek(st));
    splaytree_free(st);
}

void TestSplaytree_post_put_means_not_empty(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 10);

    CuAssertTrue(tc, !splaytree_is_empty(st));
    CuAssertTrue(tc, 1 == splaytree_count(st));
    splaytree_free(st);
}

void TestSplaytree_put_x_plus_others_then_get_x(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 5, (void *) 1);
    splaytree_put(st, (void *) 1, (void *) 1);
    splaytree_put(st, (void *) 3, (void *) 1);
    splaytree_put(st, (void *) 10, (void *) 1);

    splaytree_put(st, (void *) 15, (void *) 2);
    splaytree_put(st, (void *) 2, (void *) 3);
    splaytree_put(st, (void *) 4, (void *) 4);

    CuAssertTrue(tc, 7 == splaytree_count(st));
    CuAssertTrue(tc, 3 == (unsigned long) splaytree_get(st, (void *) 2));
    splaytree_free(st);
}

void TestSplaytree_put_4_get_2(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 15, (void *) 2);
    splaytree_put(st, (void *) 2, (void *) 3);
    splaytree_put(st, (void *) 4, (void *) 4);

    CuAssertTrue(tc, 3 == (unsigned long) splaytree_get(st, (void *) 2));
    CuAssertTrue(tc, 4 == (unsigned long) splaytree_get(st, (void *) 4));

    splaytree_free(st);
}

void TestSplaytree_get_puts_at_root(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 11, (void *) 4);
    CuAssertTrue(tc, 4 == (unsigned long) splaytree_peek(st));

    splaytree_get(st, (void *) 10);
    CuAssertTrue(tc, 1 == (unsigned long) splaytree_peek(st));

    splaytree_free(st);
}

void TestSplaytree_put_does_not_dupe(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 10, (void *) 4);
    CuAssertTrue(tc, 1 == splaytree_count(st));
    splaytree_free(st);
}

void TestSplaytree_put_dupe_doesnt_overwrite(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 10, (void *) 4);
    CuAssertTrue(tc, 1 == (unsigned long) splaytree_get(st, (void *) 10));

    splaytree_free(st);
}

void TestSplaytree_get_nonexistant_fails(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 15, (void *) 2);
    splaytree_put(st, (void *) 2, (void *) 3);
    splaytree_put(st, (void *) 4, (void *) 4);

    CuAssertTrue(tc, 0 == (unsigned long) splaytree_get(st, (void *) 5));
    CuAssertTrue(tc, 0 == (unsigned long) splaytree_get(st, (void *) 678));

    splaytree_free(st);
}

void TestSplaytree_get_when_empty_fails(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    CuAssertTrue(tc, !splaytree_get(st, (void *) 15));

    splaytree_free(st);
}

void TestSplaytree_remove_when_empty_fails(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    CuAssertTrue(tc, !splaytree_remove(st, (void *) 15));

    splaytree_free(st);
}

void TestSplaytree_remove_successully_if_exists(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 10, (void *) 1);
    splaytree_put(st, (void *) 15, (void *) 2);
    splaytree_put(st, (void *) 2, (void *) 3);
    splaytree_remove(st, (void *) 15);

    CuAssertTrue(tc, 2 == splaytree_count(st));
    CuAssertTrue(tc, 3 == (unsigned long) splaytree_get(st, (void *) 2));
    CuAssertTrue(tc, 1 == (unsigned long) splaytree_get(st, (void *) 10));

    splaytree_free(st);
}

void TestSplaytree_remove_returns_value(
    CuTest * tc
)
{
    splaytree_t *st;

    st = splaytree_initalloc(__mem_compare);

    splaytree_put(st, (void *) 15, (void *) 2);
    CuAssertTrue(tc, 2 == (unsigned long) splaytree_remove(st, (void *) 15));

    splaytree_free(st);
}
