#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>

#include <screen/screen.h>
#include "pmem.h"

int main(int argc, char *argv[])
{
	screen_context_t screen_ctx;
	screen_pixmap_t pixmap;
	screen_buffer_t buffer;
        int format_pix = SCREEN_FORMAT_RGBA8888;
	int size_pix[2] = {1280, 720};
	int stride = 4 * 1280;
	int size = stride * 720;
	void *addr = NULL;
	pmem_handle_t phandle = NULL;
	uint32_t memflag = PMEM_FLAGS_TYPED_MEM;
	int vmid = 1 << 2;
	int rc;

	addr = pmem_malloc_ext_v2(size, PMEM_GRAPHICS_FRAMEBUFFER_ID, memflag,
				  PMEM_ALIGNMENT_4K, 1 << 2, &phandle, NULL);
	if (NULL == addr) {
		printf("pmem_malloc_ext_v2 failed\n");
		goto fail;
	}

	rc = screen_create_context(&screen_ctx, 0);
	if (rc) {
		printf("screen_create_context failed with error %d (0x%08x)\n", rc, rc);
		goto fail;
	}

	rc = screen_create_pixmap(&pixmap, screen_ctx);
	if (rc) {
		printf("screen_create_pixmap failed\n");
		goto fail;
	}

	rc = screen_create_buffer(&buffer);
	if (rc) {
		printf("screen_create_buffer failed\n");
		goto fail;
	}

	rc = screen_set_buffer_property_iv(buffer, SCREEN_PROPERTY_BUFFER_SIZE, size_pix);
	if (rc) {
		printf("screen_set_buffer_property_iv(SCREEN_PROPERTY_BUFFER_SIZE) failed\n");
		goto fail;
	}

	rc = screen_set_buffer_property_iv(buffer, SCREEN_PROPERTY_FORMAT, &format_pix);
	if (rc) {
		printf("screen_set_buffer_property_iv(SCREEN_PROPERTY_FORMAT) failed\n");
		goto fail;
	}

	rc = screen_set_buffer_property_iv(buffer, SCREEN_PROPERTY_STRIDE, &stride);
	if (rc) {
		printf("screen_set_buffer_property_iv(SCREEN_PROPERTY_STRIDE) failed\n");
		goto fail;
	}

	rc = screen_set_buffer_property_iv(buffer, SCREEN_PROPERTY_SIZE, &size);
	if (rc) {
		printf("screen_set_buffer_property_iv(SCREEN_PROPERTY_SIZE) failed\n");
		goto fail;
	}

	rc = screen_set_buffer_property_pv(buffer, SCREEN_PROPERTY_EGL_HANDLE, (void**)&phandle);
	if (rc) {
		printf("screen_set_buffer_property_pv(SCREEN_PROPERTY_EGL_HANDLE) failed\n");
		goto fail;
	}

	rc = screen_attach_pixmap_buffer(pixmap, buffer);
	if (rc) {
		printf("screen_attach_pixmap_buffer failed, errno: %d\n", errno);
		goto fail;
	}

	printf("Success!!\n");
	return 0;

fail:
	printf("Fail!!\n");
	return -1;
}
