#include "edge.h"
#include "errors.h"

int check_line(const line &edge, const int &count) {
    if ((edge.dot1 < 0) || (edge.dot1 >= count) ||
        (edge.dot2 < 0) || (edge.dot2 >= count))
        return INCORRECT_FIG;
    else
        return OK;
}

int edge_copy(line &scr_edge, const line &fig_edge) {
    scr_edge = fig_edge;

    return OK;
}

int import_edge(line &edge, FILE *const f) {
    if (fscanf(f, "%d", &edge.dot1) != 1)
        return WRONG_DATA;
    if (fscanf(f, "%d", &edge.dot2) != 1)
        return WRONG_DATA;
    return OK;
}

int export_edge(FILE *const f, const line &edge) {
    fprintf(f, "%d %d\n", edge.dot1, edge.dot2);

    return OK;
}
