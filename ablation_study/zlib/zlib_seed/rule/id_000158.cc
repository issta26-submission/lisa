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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

typedef struct InDesc { const unsigned char *data; unsigned int len; } InDesc;
typedef struct OutDesc { unsigned char *out; unsigned int capacity; unsigned int written; } OutDesc;

/* in_func: provide input buffer once, then make length zero so subsequent calls return 0 */
static unsigned int my_in_cb(void *in_d, unsigned char **buf) {
    InDesc *d = (InDesc *)in_d;
    unsigned int ret = d->len;
    *buf = (unsigned char *)d->data;
    d->data = NULL;
    d->len = 0;
    return ret;
}

/* out_func: copy output to supplied buffer and advance written count */
static int my_out_cb(void *out_d, unsigned char *buf, unsigned int len) {
    OutDesc *od = (OutDesc *)out_d;
    memcpy(od->out + od->written, buf, (size_t)len);
    od->written += len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char compData[] = { 0x01u, 0x03u, 0x00u, 0xFCu, 0xFFu, (unsigned char)'A', (unsigned char)'B', (unsigned char)'C' };
    unsigned int compLen = (unsigned int)sizeof(compData);
    unsigned char outputBuf[64];
    memset(outputBuf, 0, sizeof(outputBuf));
    InDesc inDesc;
    inDesc.data = compData;
    inDesc.len = compLen;
    OutDesc outDesc;
    outDesc.out = outputBuf;
    outDesc.capacity = (unsigned int)sizeof(outputBuf);
    outDesc.written = 0;
    uLong compileFlags = zlibCompileFlags();
    (void)compileFlags;

    /* step 2: Setup - initialize inflate-back with a window buffer for raw deflate */
    unsigned char window[1 << 15];
    (void)inflateBackInit_(&strm, -15, window, zlibVersion(), (int)sizeof(z_stream));

    /* step 3: Operate (inflateBack using raw deflate stored block provided via in_cb) */
    inflateBack(&strm, my_in_cb, (void *)&inDesc, my_out_cb, (void *)&outDesc);

    /* step 4: Validate & Cleanup */
    uLong checksum = adler32(0L, outputBuf, (uInt)outDesc.written);
    (void)checksum;
    inflateBackEnd(&strm);

    return 66;
}