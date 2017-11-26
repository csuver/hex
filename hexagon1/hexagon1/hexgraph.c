//
// hexgraph - routines to manage turtle graphics for generating paths
//

#include "hex.h"
#include "hexgraph.h"

#define HEXGRAPH_NUMBER_BUF_SIZE 50
#define MAX_DOUBLE_IMAGE 30 /* this is longer than necessary s/b 20 something digits */


struct _hexgraph {
    HexList *points;
    double   direction; // angle, in degrees, the "turtle" is facing
    double   scale;
    double   offset_x, offset_y;
};


void hexgraph_copy_point(HexPoint *src, HexPoint *dst)
{
    dst->x = src->x;
    dst->y = src->y;
}

HexGraph *hexgraph_create(void)
{
    HexGraph *graph;
    HexList *points;

    graph = (HexGraph *)xalloc(sizeof(HexGraph));
    if (NULL == graph) return NULL;

    points = hexlist_create(sizeof(HexPoint));
    if (NULL == points) {
        xfree(graph);
        return NULL;
    }

    graph->points     = points;
    graph->direction  = 0.0;
    graph->scale      = 1.0;
    graph->offset_x   = 0.0;
    graph->offset_y   = 0.0;

    return graph;
}

void hexgraph_clear(HexGraph *graph)
{
    if (NULL == graph) return;

    hexlist_clear(graph->points);

    graph->direction  = 0.0;
    graph->scale      = 1.0;
    graph->offset_x   = 0.0;
    graph->offset_y   = 0.0;

    return;
}

void hexgraph_destroy(HexGraph *graph)
{
    if (NULL == graph) return;

    hexlist_destroy(graph->points);
    xfree(graph);
    return;
}

void hexgraph_set_origin(HexGraph *graph, double x, double y) 
{
    HexPoint *p = (HexPoint *)hexlist_append(graph->points);

    p->x = x;
    p->y = y;

    return;
}

void hexgraph_set_scale(HexGraph *graph, double scale)
{
    graph->scale = scale;
    return;
}

void hexgraph_set_offset(HexGraph *graph, double offset_x, double offset_y)
{
    graph->offset_x = offset_x;
    graph->offset_y = offset_y;
    return;
}

void hexgraph_set_to_continue(HexGraph *graph, HexGraph *prev)
{
    HexPoint saved;

    graph->direction = prev->direction;
    graph->scale     = prev->scale;
    graph->offset_x  = prev->offset_x;
    graph->offset_y  = prev->offset_y;

    hexgraph_copy_point((HexPoint *)hexlist_last(prev->points), &saved);
    hexgraph_copy_point(&saved, (HexPoint *)hexlist_append(graph->points));

    return;
}

HexPoint *hexgraph_scale(HexGraph *graph, HexPoint *p)
{
    p->x = (p->x * graph->scale) + graph->offset_x;
    p->y = (p->y * graph->scale) + graph->offset_y;
    return p;
}

void hexgraph_reset_to_end (HexGraph *graph) 
{
    HexPoint saved;
    
    hexgraph_copy_point((HexPoint *)hexlist_last(graph->points), &saved);
    hexlist_clear(graph->points);
    hexgraph_copy_point(&saved, (HexPoint *)hexlist_append(graph->points));

    return;
}

void hexgraph_turn(HexGraph *graph, HexGraphDir d, double angle) 
{
    if (d == RIGHT) {
        angle = -angle;
    }
    else if (d == LEFT) {
        // do nothing LEFT (counter clockwise) is normal
    }
    else {
        // also do nothing, TODO what should we do here? divide by 0?
    }

    graph->direction += TO_RADIANS(angle);

    return;
}

void hexgraph_draw(HexGraph *graph, double distance) 
{
    HexPoint *p;
    double old_x, old_y;
    double delta_x, delta_y;
    double new_x, new_y;

    // we're drawing by continuing from the last point we were at
    p = (HexPoint *)hexlist_last(graph->points);

    // if there's no "last" no points have been added
    // so we create a 0,0 origin point to start from
    if (NULL == p) {
        p = (HexPoint *)hexlist_append(graph->points);
        p->x = 0.0;
        p->y = 0.0;
    }

    old_x = p->x;
    old_y = p->y;

    delta_x = sin(graph->direction) * distance;
    delta_y = cos(graph->direction) * distance;

    new_x = old_x + delta_x;
    new_y = old_y + delta_y;

    p = (HexPoint *)hexlist_append(graph->points);
    p->x = new_x;
    p->y = new_y;

    return;
}

void hexgraph_print_path(HexGraph *graph, FILE *o)
{
    hexgraph_print_path_header(graph, o);
    hexgraph_print_path_helper(graph, o);
    fputs("\"\n", o);
    hexgraph_print_path_tail(graph, o);
}

void hexgraph_print_closed_path(HexGraph *graph, FILE *o) 
{
    hexgraph_print_path_header(graph, o);
    hexgraph_print_path_helper(graph, o);
    fputs(" Z\"\n", o);
    hexgraph_print_path_tail(graph, o);
}

void hexgraph_bounds(HexGraph *graph, HexPoint *min, HexPoint *max)
{
    double       min_x, min_y;
    double       max_x, max_y;
    HexListIter *iter = hexlist_iterate(graph->points);
    HexPoint    *p;

    p = (HexPoint *)hexlist_start(iter);
    max_x = min_x = p->x;
    max_y = min_y = p->y;
    while (NULL != (p = (HexPoint *)hexlist_next(iter)))
    {
        if (min_x > p->x) min_x = p->x;
        if (min_y > p->y) min_y = p->y;
        if (max_x < p->x) max_x = p->x;
        if (max_y < p->y) max_y = p->y;
    }

    min->x = min_x;
    min->y = min_y;
    max->x = max_x;
    max->y = max_y;

    return;
}

void hexgraph_print_g_header(HexGraph *graph, HexPoint *min, FILE *o)
{
    char     tempnumber[HEXGRAPH_NUMBER_BUF_SIZE];
    HexPoint scaled_p;

    hexgraph_copy_point(min, &scaled_p);
    hexgraph_scale(graph, &scaled_p);

    fputs("<g transform = \"translate(", o);
    fputs(hexgraph_format_number(tempnumber, sizeof(tempnumber), -scaled_p.x), o);
    fputs(", ", o);
    fputs(hexgraph_format_number(tempnumber, sizeof(tempnumber), -scaled_p.y), o);
    fputs(")\">\n", o);

    return;
}

void hexgraph_print_g_tail(HexGraph *graph, FILE *o)
{
    fputs("</g>\n", o);
    return;
}

void hexgraph_print_path_header(HexGraph *graph, FILE *o) 
{
    fputs("  <path\n    d=\"", o);
}

void hexgraph_print_path_helper(HexGraph *graph, FILE *o) 
{
    char        *header = "M ";
    HexListIter *iter = hexlist_iterate(graph->points);
    HexPoint    *p, scaled_p;
    char         tempnumber[HEXGRAPH_NUMBER_BUF_SIZE];

    for (p=(HexPoint *)hexlist_start(iter); NULL != p; p=(HexPoint *)hexlist_next(iter)) {
        hexgraph_copy_point(p, &scaled_p);
        hexgraph_scale(graph, &scaled_p);
        fputs(header, o);
        fputs(hexgraph_format_number(tempnumber, sizeof(tempnumber), scaled_p.x), o);
        fputs(" ", o);
        fputs(hexgraph_format_number(tempnumber, sizeof(tempnumber), scaled_p.y), o);
        header = " L ";
    }
}

 //  this format function is limited to values reasonable to expect in
 //  drawing for nominally real ranges. So +/-1,000,000 and no more than
 //  6 decimal digits of precision
char *hexgraph_format_number(char *buf, SIZE size, double v)
{
    char *cp = buf;
    SIZE  remaining = size;
    char  tempbuf[2*MAX_DOUBLE_IMAGE+1];
    int    len;

    assert(size >= 5); // sign, 1 digit, decimal, 1 digit, null terminator
    assert(1000000.0 >= v >= -1000000.0);

    // jut brute force
    // char * restrict str, size_t size, const char * restrict format, ...
    len = snprintf(tempbuf, 2 * MAX_DOUBLE_IMAGE, "%6.2f", v);

    // we break on the first non-zero *trailing* digit 
    for (cp = &tempbuf[len - 1];  (cp > tempbuf) && *cp == '0'; /* in loop cp-- */)
    {
        cp--;
        if (*cp == '.') {
            // we break on the '.' in the case of xxx.0000
            break;
        }
    }
    if (*cp == '.') {
        len = cp - tempbuf; // we don't include the decimal point if there is no fraction
    }
    else {
        assert('0' < *cp <= '9');
        len = cp - tempbuf + 1;
    }

    /* skip leading spaces, if any */
    assert(cp > tempbuf && *cp);
    for (cp = tempbuf; *cp == ' '; cp++) {
        len--;
    }

    // copy the good bits to the output buffer
    memcpy(buf, cp, len);
    buf[len] = '\0';

    return buf;
}

void hexgraph_print_path_tail(HexGraph *graph, FILE *o) 
{
    fputs("    stroke=\"black\"", o);
    fputs(" stroke-width=\"0.1\"", o);
    fputs(" fill=\"none\"", o);
    fputs("\n  />\n", o);
}





