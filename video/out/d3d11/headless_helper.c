#include "headless_helper.h"
#include "video/out/vo.h"

int d3d11_headless_control(struct vo* vo, int* events, int request, void* arg) {
    int width_new = libmpv_dxgi_get_width(),
        height_new = libmpv_dxgi_get_height();
    if (vo->dwidth != width_new || vo->dheight != height_new) {
        vo->dwidth = width_new;
        vo->dheight = height_new;
        (*events) |= VO_EVENT_RESIZE;
    }
    return VO_TRUE;
}