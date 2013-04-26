/*
 
Copyright (c) 2011, Willem-Hendrik Thiart
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The names of its contributors may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL WILLEM-HENDRIK THIART BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "splaytree.h"

#define LEFT 0
#define RIGHT 1

typedef struct tree_node_s tree_node_t;

struct tree_node_s
{
    tree_node_t *left, *right;
    void *key, *value;
};

splaytree_t *splaytree_initalloc(
    int (*cmp) (const void *,
                const void *)
)
{
    splaytree_t *st;

    st = malloc(sizeof(splaytree_t));
    memset(st, 0, sizeof(splaytree_t));
    st->cmp = cmp;
    return st;
}

static void __free_node(
    tree_node_t * node
)
{
    if (node)
    {
        __free_node(node->left);
        __free_node(node->right);
        free(node);
    }
}

void splaytree_free(
    splaytree_t * st
)
{
    __free_node(st->root);
    free(st);
}

static tree_node_t *__init_node(
    void *key,
    void *value
)
{
    tree_node_t *new;

    new = malloc(sizeof(tree_node_t));
    new->left = new->right = NULL;
    new->key = key;
    new->value = value;
    return new;
}

static void __rotate_right(
    tree_node_t ** pa
)
{
    tree_node_t *child;

    child = (*pa)->left;
    assert(child);
    (*pa)->left = child->right;
    child->right = *pa;
    *pa = child;
}

static void __rotate_left(
    tree_node_t ** pa
)
{
    tree_node_t *child;

    child = (*pa)->right;
    assert(child);
    (*pa)->right = child->left;
    child->left = *pa;
    *pa = child;
}

/**
 * bring this value to the top
 * */
static tree_node_t *__splay(
    splaytree_t * st,
    int update_if_not_found,
    tree_node_t ** gpa,
    tree_node_t ** pa,
    tree_node_t ** child,
    const void *key
)
{
    int cmp;

    tree_node_t *next;

    if (!(*child))
    {
        return NULL;
    }

    cmp = st->cmp((*child)->key, key);

    if (cmp == 0)
    {
        next = *child;
    }
    else if (cmp > 0)
    {
        next =
            __splay(st, update_if_not_found, pa, child, &(*child)->left, key);
    }
    else if (0 > cmp)
    {
        next =
            __splay(st, update_if_not_found, pa, child, &(*child)->right, key);
    }

    if (!next)
    {
        if (update_if_not_found)
        {
            next = *child;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (next != *child)
            return next;
    }

    if (!pa)
        return next;

    if (!gpa)
    {
        /* zig left */
        if ((*pa)->left == next)
        {
            __rotate_right(pa);
        }
        /* zig right */
        else
        {
            __rotate_left(pa);
        }
        return next;
    }

    assert(gpa);

    /* zig zig left */
    if ((*pa)->left == next && (*gpa)->left == *pa)
    {
        __rotate_right(pa);
        __rotate_right(gpa);
    }
    /* zig zig right */
    else if ((*pa)->right == next && (*gpa)->right == *pa)
    {
        __rotate_left(pa);
        __rotate_left(gpa);
    }
    /* zig zag right */
    else if ((*pa)->right == next && (*gpa)->left == *pa)
    {
        // FIXME: tests don't cover this conditional
        __rotate_left(pa);
        __rotate_right(gpa);
    }
    /* zig zag left */
    else if ((*pa)->left == next && (*gpa)->right == *pa)
    {
        __rotate_right(pa);
        __rotate_left(gpa);
    }

    return next;
}

int splaytree_is_empty(
    splaytree_t * st
)
{
    return NULL == st->root;
}

void *splaytree_remove(
    splaytree_t * st,
    const void *key
)
{
    tree_node_t *root, *left_highest;

    void *val;

    /*  make removed node the root */
    if (!splaytree_get(st, key))
    {
        return NULL;
    }

    root = st->root;
    val = root->value;

    assert(0 < st->count);
    assert(root->key == key);

    /* get left side's most higest value node */
    if ((left_highest = root->left))
    {
        tree_node_t *prev = root;

        while (left_highest->right)
        {
            prev = left_highest;
            left_highest = left_highest->right;
        }

        /* do the swap */
        prev->right = NULL;
        st->root = left_highest;
        left_highest->left = root->left;
        left_highest->right = root->right;
    }
    /* there is no left */
    else
    {
        assert(root);
        st->root = root->right;
    }

    st->count--;

    assert(root != st->root);
    free(root);

    return val;
}

/**
 * get this item referred to by key. Slap it as root.
 *
 */
void *splaytree_get(
    splaytree_t * st,
    const void *key
)
{
    tree_node_t *node;

    node = __splay(st, 0, NULL, NULL, (tree_node_t **) & st->root, key);

    return node ? node->value : NULL;
}

#if 0
static int __count_recurse(
    tree_node_t * node
)
{
    if (!node)
    {
        return 0;
    }
    else
    {
        return 1 + __count(node->left) + __count(node->right);
    }
}
#endif

int splaytree_count(
    splaytree_t * st
)
{
#if 1
    return st->count;
#else
    return __count(st->root);
#endif
}

void *splaytree_peek(
    splaytree_t * st
)
{
    return st->root ? ((tree_node_t *) st->root)->value : NULL;
}

void splaytree_put(
    splaytree_t * st,
    void *key,
    void *value
)
{
    tree_node_t *new;

    int cmp;

    if (!st->root)
    {
        st->root = __init_node(key, value);
        st->count++;
        goto exit;
    }

    new = __splay(st, 1, NULL, NULL, (tree_node_t **) & st->root, key);

    cmp = st->cmp(((tree_node_t *) st->root)->key, key);

    if (cmp != 0)
    {
        new = __init_node(key, value);

        if (0 < cmp)
        {
            new->right = st->root;
            new->left = new->right->left;
            new->right->left = NULL;
        }
        else
        {
            new->left = st->root;
            new->right = new->left->right;
            new->left->right = NULL;
        }

        st->count++;
    }

    st->root = new;

  exit:
    return;
}
