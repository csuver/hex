//
// hexlist - routines to manage lists of things
//

#include "hex.h"
#include "hexlist.h"

#define HEXLIST_ITEM_DUMMY_SPACE 4

struct _hxl_item {
    struct _hxl_item *next;
    BYTE data[HEXLIST_ITEM_DUMMY_SPACE];
};

struct _hxl_head {
    struct _hxl_item *head;
    struct _hxl_item *tail;
    SIZE              datasize;
    SIZE              count;
};

struct _hxl_iter {
    struct _hxl_head *list;
    struct _hxl_item *curr;
};

HexList *hexlist_create(SIZE datasize) {
    HexList *list;

    list = (HexList *)xalloc(sizeof(HexList));
    if (NULL == list) return NULL;

    return hexlist_init_static(list, datasize);
}

HexList *hexlist_init_static(HexList *list, SIZE datasize)
{
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    list->datasize = datasize;

    return list;
}

void hexlist_clear(HexList *list)
{
    HexListItem *item, *next;

    if (!list) return;

    for (next = NULL, item = list->head; item != NULL; item = next) {
        next = item->next;
        xfree(item);
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    return;
}

void hexlist_destroy(HexList *list)
{
    hexlist_clear(list);
    xfree(list);
    return;
}

BYTE *hexlist_append(HexList *list)
{
    HexListItem *item;

    item = (HexListItem *)xalloc(list->datasize + sizeof(HexListItem) - HEXLIST_ITEM_DUMMY_SPACE);
    item->next = NULL;

    if (list->tail) {
        list->tail->next = item;
    }
    else {
        list->head = item;
    }

    list->tail = item;
    list->count++;

    return &(item->data[0]);
}

BYTE *hexlist_first(HexList *list)
{
    if (NULL == list) return NULL;
    if (NULL == list->head) return NULL;
    return &(list->head->data[0]);
}

BYTE *hexlist_last(HexList *list)
{
    if (NULL == list) return NULL;
    if (NULL == list->tail) return NULL;
    return &(list->tail->data[0]);
}

HexListIter *hexlist_iterate(HexList *list)
{
    HexListIter *iter;

    if (!list) return NULL;

    iter = (HexListIter *)xalloc(sizeof(HexListIter));
    iter->list = list;
    iter->curr = NULL;

    return iter;
}

BYTE *hexlist_start(HexListIter *iter)
{
    iter->curr = iter->list->head;
    if (iter->curr == NULL) {
        return NULL;
    }
    else {
        return &(iter->curr->data[0]);
    }
}

BYTE *hexlist_next(HexListIter *iter)
{
    if (NULL == iter->curr) return NULL;
    iter->curr = iter->curr->next;
    if (iter->curr == NULL) {
        return NULL;
    }
    else {
        return &(iter->curr->data[0]);
    }
}

void hexlist_close_iterator(HexListIter *iter)
{
    if (NULL == iter) return;
    xfree(iter);
    return;
}
