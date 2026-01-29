#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 599
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input context for inflateBack in_func */
typedef struct {
    unsigned char *data;
    unsigned int pos;
    unsigned int remain;
} in_ctx_t;

/* Helper output context for inflateBack out_func */
typedef struct {
    unsigned char *buf;
    unsigned int size;
    unsigned int pos;
} out_ctx_t;

/* in_func: provide a pointer to available input and return its size */
static unsigned my_in_func(void *in_desc, unsigned char **buf) {
    in_ctx_t *c = (in_ctx_t *)in_desc;
    if (c->remain == 0) {
        *buf = NULL;
        return 0;
    }
    *buf = c->data + c->pos;
    unsigned ret = c->remain;
    c->pos += ret;
    c->remain = 0;
    return ret;
}

/* out_func: consume output produced by inflateBack; return 0 on success */
static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    out_ctx_t *o = (out_ctx_t *)out_desc;
    unsigned int copy_len = len;
    if (o->pos + copy_len > o->size) {
        /* Truncate if output buffer would overflow */
        copy_len = o->size - o->pos;
    }
    if (copy_len > 0) {
        memcpy(o->buf + o->pos, buf, copy_len);
        o->pos += copy_len;
    }
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    z_stream src;
    memset(&src, 0, sizeof(src));
    z_stream dst;
    memset(&dst, 0, sizeof(dst));
    const char * ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    const unsigned int IN_LEN = 64;
    const unsigned int OUT_LEN = 128;
    Bytef * inBuf = (Bytef *)malloc((size_t)IN_LEN);
    memset(inBuf, 'Z', IN_LEN);
    uLong inLen = (uLong)IN_LEN;
    Bytef * outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, OUT_LEN);

    /* step 2: Initialize streams */
    inflateInit_(&src, ver, (int)sizeof(z_stream));
    src.next_in = inBuf;
    src.avail_in = (uInt)inLen;
    src.next_out = outBuf;
    src.avail_out = (uInt)OUT_LEN;

    inflateInit_(&dst, ver, (int)sizeof(z_stream));

    /* step 3: Configure / Operate (copy stream, switch to inflateBack, run back inflate, write to gz) */
    inflateCopy(&dst, &src);
    /* finish the copied inflate state and reinitialize dst for inflateBack usage */
    inflateEnd(&dst);

    /* prepare window for inflateBackInit_ */
    const int WINDOW_BITS = 15;
    const unsigned int WINDOW_SIZE = 1u << WINDOW_BITS;
    unsigned char *window = (unsigned char *)malloc(WINDOW_SIZE);
    memset(window, 0, WINDOW_SIZE);

    inflateBackInit_(&dst, WINDOW_BITS, window, ver, (int)sizeof(z_stream));

    /* prepare in/out contexts and call inflateBack */
    in_ctx_t in_ctx;
    in_ctx.data = inBuf;
    in_ctx.pos = 0;
    in_ctx.remain = (unsigned int)inLen;

    out_ctx_t out_ctx;
    out_ctx.buf = outBuf;
    out_ctx.size = OUT_LEN;
    out_ctx.pos = 0;

    inflateBack(&dst, (in_func)my_in_func, &in_ctx, (out_func)my_out_func, &out_ctx);

    inflateBackEnd(&dst);

    gzFile gz = gzopen("tmp_test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'A');
    gzclose(gz);

    /* step 4: Validate/cleanup */
    (void)ver;
    (void)flags;
    inflateEnd(&src);
    free(window);
    free(inBuf);
    free(outBuf);
    return 66;
}