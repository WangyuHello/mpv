#include "config.h"
#include "video/out/gpu/hwdec.h"
#include "video/out/gpu/video.h"
#include "video/out/libmpv.h"
#include "libmpv/render_dxgi.h"

#include <windows.h>

// TODO: this should be exposed vo_d3d11, 
// and should be retrived when "control" method called.
typedef struct d3d11_headless_priv {
    int width;
    int height;
    // pointer to user's swapchain pointer and d3d11 device pointer
    void* swc_out; // IDXGISwapChain**
    void* dev_out; // ID3D11Device**
} d3d11_headless_priv;
static d3d11_headless_priv* pPriv = NULL;

static int init(struct render_backend* ctx, mpv_render_param* params)
{
    MessageBox(NULL, L"libmpv_headless init", NULL, MB_OK);
    char* api = get_mpv_render_param(params, MPV_RENDER_PARAM_API_TYPE, NULL);
    if (!api) {
        return MPV_ERROR_INVALID_PARAMETER;
    }

    if (strcmp(api, MPV_RENDER_API_TYPE_DXGI) != 0) {
        return MPV_ERROR_NOT_IMPLEMENTED;
    }

    pPriv = ctx->priv = talloc_zero(NULL, d3d11_headless_priv);
    pPriv->width = 320;
    pPriv->height = 240;

    //TODO: define a new init param type
    mpv_dxgi_init_param* init_param = get_mpv_render_param(params, MPV_RENDER_PARAM_DXGI_INIT_PARAM, NULL);
    if (init_param != NULL) {
        pPriv->swc_out = init_param->swc_out;
        pPriv->dev_out = init_param->dev_out;
    }
    return 0;
}

static bool check_format(struct render_backend* ctx, int imgfmt)
{
    return true;
}

static int set_parameter(struct render_backend* ctx, mpv_render_param param) {
    // TODO: use this method to update render size things.
    mpv_dxgi_render_param* render_param = param.data;
    d3d11_headless_priv* priv = ctx->priv;
    priv->width = render_param->width;
    priv->height = render_param->height;
    return 0;
}

static void destroy(struct render_backend* ctx)
{
    // TODO: destroy me
}

const struct render_backend_fns render_backend_dxgi = {
    .init = init,
    .check_format = check_format,
    .set_parameter = set_parameter,
    .reconfig = NULL,
    .reset = NULL,
    .update_external = NULL,
    .resize = NULL,
    .get_target_size = NULL,
    .render = NULL,
    .get_image = NULL,
    .screenshot = NULL,
    .perfdata = NULL,
    .destroy = destroy,
};