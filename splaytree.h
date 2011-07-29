/*
 * =====================================================================================
 *
 *       Filename:  splaytree.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/26/11 22:11:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

typedef struct
{
    void *root;
    int (
    *cmp
    )   (
    const void *,
    const void *
    );
    int count;
} splaytree_t;


splaytree_t *splaytree_initalloc(
    int (*cmp) (const void *,
                const void *)
);

void splaytree_free(
    splaytree_t * st
);

int splaytree_is_empty(
    splaytree_t * st
);


void *splaytree_remove(
    splaytree_t * st,
    const void *key
);


void *splaytree_get(
    splaytree_t * st,
    const void *key
);


int splaytree_count(
    splaytree_t * st
);


void *splaytree_peek(
    splaytree_t * st
);

void splaytree_put(
    splaytree_t * st,
    void *key,
    void *value
);
