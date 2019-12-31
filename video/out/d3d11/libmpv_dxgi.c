#include "config.h"
#include "video/out/gpu/hwdec.h"
#include "video/out/gpu/video.h"
#include "video/out/libmpv.h"
#include "libmpv/render_dxgi.h"

#include <windows.h>
#include <d3d11.h>

// This is exposed vo_d3d11, 
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

    mpv_dxgi_init_param* init_param = get_mpv_render_param(params, MPV_RENDER_PARAM_DXGI_INIT_PARAM, NULL);
    if (init_param != NULL) {
        pPriv->swc_out = init_param->swc_out;
        pPriv->dev_out = init_param->dev_out;
    }
    return 0;
}

static int set_parameter(struct render_backend* ctx, mpv_render_param param) {
    // update render size.
    mpv_dxgi_render_param* render_param = param.data;
    d3d11_headless_priv* priv = ctx->priv;
    priv->width = render_param->width;
    priv->height = render_param->height;
    return 0;
}

static void destroy(struct render_backend* ctx)
{
    talloc_free(ctx->priv);
}

/**
 * Following methods basically do nothing but prevent unexpected crash.
 */
static bool check_format(struct render_backend* ctx, int imgfmt)
{
    return true;
}
static void reconfig(struct render_backend* ctx, struct mp_image_params* params)
{
    return;
}
static void reset(struct render_backend* ctx)
{
    return;
}
static void update_external(struct render_backend* ctx, struct vo* vo)
{
    return;
}
static void resize(struct render_backend* ctx, struct mp_rect* src,
    struct mp_rect* dst, struct mp_osd_res* osd)
{
    return;
}
static int get_target_size(struct render_backend* ctx, mpv_render_param* params,
    int* out_w, int* out_h)
{
    return 0;
}
static int render(struct render_backend* ctx, mpv_render_param* params,
    struct vo_frame* frame)
{
    return 0;
}
static void screenshot(struct render_backend* ctx, struct vo_frame* frame,
    struct voctrl_screenshot* args)
{
    return;
}
static void perfdata(struct render_backend* ctx,
    struct voctrl_performance_data* out)
{
    return;
}

const struct render_backend_fns render_backend_dxgi = {
    .init = init,
    .check_format = check_format,
    .set_parameter = set_parameter,
    .reconfig = reconfig,
    .reset = reset,
    .update_external = update_external,
    .resize = resize,
    .get_target_size = get_target_size,
    .render = render,
    .get_image = NULL,
    .screenshot = screenshot,
    .perfdata = perfdata,
    .destroy = destroy,
};

/**
 * Following methods are used by vo_d3d11_headless to retrive informations
 * passed in from libmpv fake render backend.
 */
 // pass out the IDXGISwapChain to API user
bool libmpv_dxgi_swc_out(void* pIDXGISwapChain)
{
    IDXGISwapChain** _swc_out = pPriv->swc_out;
    if (pPriv->swc_out != NULL) {
        *_swc_out = (IDXGISwapChain*)pIDXGISwapChain;
        return true;
    }
    return false;
}

//pass out the ID3D11Device to API user
bool libmpv_dxgi_dev_out(void* pID3D11Device)
{
    ID3D11Device** _dev_out = pPriv->dev_out;
    if (pPriv->dev_out != NULL) {
        *_dev_out = (ID3D11Device*)pID3D11Device;
        return true;
    }
    return false;
}

int libmpv_dxgi_get_width(void) {
    return pPriv->width;
}

int libmpv_dxgi_get_height(void) {
    return pPriv->height;
}