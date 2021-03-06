//
// hexagon1.c : Generates points for a interlocking hexagons
//

#include "hex.h"

#define SIMPLE  1
#define TABBED  2
#define SURFACE 3
static int g_hex_type = SURFACE;

void print_simple_hex(FILE *o, double R, double scale);
void print_tabbed_hex(FILE *o, double R, double scale);
void print_surface_hex(FILE *o, double R, double scale);

int main()
{
    double radius = 1.732051;
    double scale = 50;
    int    ret = 0;

    switch (g_hex_type) {
    case SIMPLE:
        print_simple_hex(stdout, radius, scale);
        break;
    case TABBED:
        print_tabbed_hex(stdout, radius, scale);
        break;
    case SURFACE:
        print_surface_hex(stdout, radius, scale);
        break;
    default:
        fputs("\n\n!!!! invalid drawing type\n\n", stderr);
        ret = 1;
        break;
    }
    return ret;
}

void print_simple_hex(FILE *o, double R, double scale)
{
    HexGraph *path = hexgraph_create();
    hexgraph_set_scale(path, scale);

    // SIMPLE HEX
    hexgraph_set_origin(path, R, R);
    hexgraph_draw(path, R);
    hexgraph_turn(path, RIGHT, 60);
    hexgraph_draw(path, R);
    hexgraph_turn(path, RIGHT, 60);
    hexgraph_draw(path, R);
    hexgraph_turn(path, RIGHT, 60);
    hexgraph_draw(path, R);
    hexgraph_turn(path, RIGHT, 60);
    hexgraph_draw(path, R);

    // OUTPUT POINTS
    hexgraph_print_closed_path(path, stdout);

    return;
}

#define PATH_MAX 8
HexGraph *g_paths[PATH_MAX];
int       g_path_count = 0;

HexGraph *add_path(double scale)
{
    HexGraph *path = NULL;
    
    if (g_path_count < PATH_MAX) {
        path = hexgraph_create();
        hexgraph_set_scale(path, scale);
        g_paths[g_path_count] = path;
        g_path_count++;
    }

    return path;
}

void unadd_path(HexGraph *path)
{
    assert(path != NULL);
    assert(g_path_count > 0);
    assert(g_paths[g_path_count - 1] == path);
    hexgraph_destroy(path);
    g_path_count--;
    g_paths[g_path_count] = NULL;
}

void print_tabbed_hex(FILE *o, double R, double scale)
{
    double tab_wide   = 0.5;
    double tab_narrow = 0.3125;
    double tab_height = 0.3125;

    double d_corner2tab    = (R - tab_narrow) / 2.0;
    double d_tab_delta     = (tab_wide - tab_narrow) / 2.0;
    double d_tab_angle     = atan2(tab_height, d_tab_delta);
    double d_tab_side_long = tab_height / sin(d_tab_angle);

    HexGraph *path, *nextpath;
    HexPoint  min_pt, max_pt;
    HexPoint  min_temp, max_temp;
    int ii;

    path = add_path(scale);

    // HEX WITH TABS
    for (int s = 0; s<3; s++) {
        // innsy
        hexgraph_draw(path, d_corner2tab);
        hexgraph_turn(path, RIGHT, 107);
        hexgraph_draw(path, d_tab_side_long);
        hexgraph_turn(path, LEFT, 107);
        hexgraph_draw(path, tab_wide);
        hexgraph_turn(path, LEFT, 107);
        hexgraph_draw(path, d_tab_side_long);
        hexgraph_turn(path, RIGHT, 107);
        hexgraph_draw(path, d_corner2tab);
        hexgraph_turn(path, RIGHT, 60);

        // pass the batton to the next path
        nextpath = add_path(scale);
        hexgraph_set_to_continue(nextpath, path);
        path = nextpath;

        // outsy
        hexgraph_draw(path, d_corner2tab);
        hexgraph_turn(path, LEFT, 107);
        hexgraph_draw(path, d_tab_side_long);
        hexgraph_turn(path, RIGHT, 107);
        hexgraph_draw(path, tab_wide);
        hexgraph_turn(path, RIGHT, 107);
        hexgraph_draw(path, d_tab_side_long);
        hexgraph_turn(path, LEFT, 107);
        hexgraph_draw(path, d_corner2tab);
        hexgraph_turn(path, RIGHT, 60);

        // pass the batton to the next path
        nextpath = add_path(scale);
        hexgraph_set_to_continue(nextpath, path);
        path = nextpath;
    }

    // we create one empty path and it's uglier to change the loop than just drop it here
    unadd_path(path);

    assert(g_path_count > 0);

    // find the min bounds of the set of paths
    path = g_paths[0];
    hexgraph_bounds(path, &min_pt, &max_pt);
    for (ii=1; ii<g_path_count; ii++) {
        path = g_paths[ii];
        hexgraph_bounds(path, &min_temp, &max_temp);
        if (min_pt.x > min_temp.x) min_pt.x = min_temp.x;
        if (min_pt.y > min_temp.y) min_pt.y = min_temp.y;
    }

    hexgraph_print_g_header(g_paths[0], &min_pt, o);

    // print the paths
    for (ii = 0; ii<g_path_count; ii++) {
        path = g_paths[ii];
        hexgraph_print_path(path, o);
    }

    hexgraph_print_g_tail(g_paths[0], o);

    return;
}

void print_surface_hex(FILE *o, double R, double scale)
{

}