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
//<ID> 592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptor for the simple in/out functions used with inflateBack */
typedef struct {
    unsigned char *p;
    unsigned int len;
    unsigned int off;
} BufDesc;

/* in_func: provide a pointer to available input bytes */
static unsigned int fb_in(void *in_desc, unsigned char **buf) {
    BufDesc *d = (BufDesc *)in_desc;
    *buf = d->p + d->off;
    unsigned int avail = d->len - d->off;
    d->off += avail;
    return avail;
}

/* out_func: consume output bytes by copying them into a destination buffer */
static unsigned int fb_out(void *out_desc, unsigned char *buf, unsigned int len) {
    BufDesc *d = (BufDesc *)out_desc;
    memcpy(d->p + d->off, buf, len);
    d->off += len;
    return len;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    const char *ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags; /* unused but retained to match original sequence */

    Bytef *input = (Bytef *)malloc((size_t)256);
    memset(input, 'A', 256);
    uLong inputLen = (uLong)256;

    uLong bound = compressBound(inputLen);
    uLongf compLen = (uLongf)bound; /* initialize dest length to available size */
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    /* step 2: Compress data and write a byte to a gz file */
    compress2(compBuf, &compLen, input, inputLen, 6);
    gzFile gz = gzopen("tmp_test_zlib_api.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    /* step 3: Initialize inflate streams and perform copying and back inflate call */
    z_stream src;
    memset(&src, 0, sizeof(src));
    z_stream dst;
    memset(&dst, 0, sizeof(dst));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));

    inflateInit_(&src, ver, (int)sizeof(z_stream));
    inflateCopy(&dst, &src);

    unsigned char *window = (unsigned char *)malloc(32);
    memset(window, 0, 32);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    BufDesc inDesc;
    inDesc.p = compBuf;
    inDesc.len = (unsigned int)compLen;
    inDesc.off = 0;

    unsigned int outBufSize = (unsigned int)(inputLen * 4 + 16);
    unsigned char *outBuf = (unsigned char *)malloc(outBufSize);
    BufDesc outDesc;
    outDesc.p = outBuf;
    outDesc.len = outBufSize;
    outDesc.off = 0;

    inflateBack(&backStrm, (in_func)fb_in, &inDesc, (out_func)fb_out, &outDesc);

    /* step 4: Validate / Cleanup */
    inflateBackEnd(&backStrm);
    inflateEnd(&src);
    inflateEnd(&dst);

    free(input);
    free(compBuf);
    free(window);
    free(outBuf);

    /* API sequence test completed successfully */
    return 66;
}