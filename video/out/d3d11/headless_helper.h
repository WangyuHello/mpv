#include <stdbool.h>

// methods defined in libmpv_dxgi.c
bool libmpv_dxgi_swc_out(void* pIDXGISwapChain);
bool libmpv_dxgi_dev_out(void* pID3D11Device);
int libmpv_dxgi_get_width(void);
int libmpv_dxgi_get_height(void);

int d3d11_headless_control(struct vo* vo, int* events, int request, void* arg);