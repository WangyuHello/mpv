#ifndef MPV_CLIENT_API_RENDER_DXGI_H_
#define MPV_CLIENT_API_RENDER_DXGI_H_
#include "render.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    typedef struct mpv_dxgi_init_param {
        void* swc_out;
        void* dev_out;
    } mpv_dxgi_init_param;

    typedef struct mpv_dxgi_render_param {
        int width;
        int height;
    } mpv_dxgi_render_param;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MPV_CLIENT_API_RENDER_DXGI_H_
