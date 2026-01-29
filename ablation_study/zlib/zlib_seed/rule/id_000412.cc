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
//<ID> 412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input descriptor for inflateBack in_func */
struct in_desc {
    const unsigned char *buf;
    unsigned int len;
    unsigned int pos;
};

/* in_func: provide a pointer to available input and return number of bytes provided */
static unsigned my_in_func(void *in_desc_void, unsigned char **bufptr) {
    struct in_desc *d = (struct in_desc *)in_desc_void;
    if (d->pos >= d->len) {
        *bufptr = NULL;
        return 0;
    }
    *bufptr = (unsigned char *)(d->buf + d->pos);
    unsigned int avail = d->len - d->pos;
    d->pos = d->len;
    return avail;
}

/* out_func: copy decompressed chunk to the provided output buffer (out_desc points to destination) */
static int my_out_func(void *out_desc, unsigned char *data, unsigned int len) {
    void *dst = out_desc;
    memcpy(dst, data, (size_t)len);
    return 0;
}

int test_zlib_api_sequence() {
    z_stream defl;
    memset(&defl, 0, sizeof(defl));
    z_stream infl;
    memset(&infl, 0, sizeof(infl));
    z_stream copy;
    memset(&copy, 0, sizeof(copy));

    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    deflateInit_(&defl, 6, zlibVersion(), (int)sizeof(z_stream));
    defl.next_in = (Bytef *)input;
    defl.avail_in = (uInt)inputLen;
    defl.next_out = compBuf;
    defl.avail_out = (uInt)bound;
    defl.adler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    deflate(&defl, Z_FINISH);

    inflateInit_(&infl, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&copy, &infl);

    gz_header gzhead;
    memset(&gzhead, 0, sizeof(gzhead));
    inflateGetHeader(&infl, &gzhead);

    copy.next_in = compBuf;
    copy.avail_in = (uInt)defl.total_out;
    copy.next_out = (Bytef *)input;
    copy.avail_out = (uInt)inputLen;

    struct in_desc in_d;
    in_d.buf = compBuf;
    in_d.len = (unsigned int)defl.total_out;
    in_d.pos = 0;

    inflateBack(&copy, (in_func)my_in_func, (void *)&in_d, (out_func)my_out_func, (void *)input);

    deflateEnd(&defl);
    inflateEnd(&infl);
    inflateEnd(&copy);
    free(compBuf);
    return 66;
}