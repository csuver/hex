#pragma once
//
// hexlist - routines to manage lists of things
//

#include "hex.h"

typedef struct _hxl_head HexList;
typedef struct _hxl_item HexListItem;
typedef struct _hxl_iter HexListIter;

HexList     *hexlist_create(SIZE datasize);
HexList     *hexlist_init_static(HexList *list, SIZE datasize);
void         hexlist_clear(HexList *list);
void         hexlist_destroy(HexList *list);

BYTE        *hexlist_append(HexList *list);
BYTE        *hexlist_first(HexList *list);
BYTE        *hexlist_last(HexList *list);

HexListIter *hexlist_iterate(HexList *list);
BYTE        *hexlist_start(HexListIter *iter);
BYTE        *hexlist_next(HexListIter *iter);
void         hexlist_close_iterator(HexListIter *iter);

