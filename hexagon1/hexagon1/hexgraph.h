//
// hexgraph - turtle graphics for generating paths
//


#pragma once

#include "hex.h"

typedef struct _hexgraph HexGraph;
typedef struct _hexpoint HexPoint;

struct _hexpoint {
    double x;
    double y;
};

#define HEXGRAPH_NUMBER_BUF_SIZE 50

//static double PI = 3.1415926535897932384626433832795;
//#define TO_RADIANS(angle) ((angle/180)*PI)

typedef enum _hexgraph_dir { LEFT, RIGHT } HexGraphDir;

void      hexgraph_copy_point(HexPoint *src, HexPoint *dst);
HexGraph *hexgraph_create(void);
void      hexgraph_clear(HexGraph *graph);
void      hexgraph_destroy(HexGraph *graph);
void      hexgraph_set_origin(HexGraph *graph, double x, double y);
void      hexgraph_set_scale(HexGraph *graph, double scale);
void      hexgraph_set_offset(HexGraph *graph, double offset_x, double offset_y);
void      hexgraph_set_to_continue(HexGraph *graph, HexGraph *prev);
HexPoint *hexgraph_scale(HexGraph *graph, HexPoint *p);
void      hexgraph_reset_to_end(HexGraph *graph);
void      hexgraph_turn(HexGraph *graph, HexGraphDir d, double angle);
void      hexgraph_draw(HexGraph *graph, double distance);
void      hexgraph_print_path(HexGraph *graph, FILE *o);
void      hexgraph_print_closed_path(HexGraph *graph, FILE *o);
void      hexgraph_bounds(HexGraph *graph, HexPoint *min, HexPoint *max);
void      hexgraph_print_g_header(HexGraph *graph, HexPoint *min, FILE *o);
void      hexgraph_print_g_tail(HexGraph *graph, FILE *o);
void      hexgraph_print_path_header(HexGraph *graph, FILE *o);
void      hexgraph_print_path_helper(HexGraph *graph, FILE *o);
char     *hexgraph_format_number(char *buf, SIZE size, double v);
void      hexgraph_print_path_tail(HexGraph *graph, FILE *o);



