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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptor types and callback implementations for inflateBack */
typedef struct {
    const unsigned char *buf;
    unsigned long avail;
} in_desc_t;

typedef struct {
    unsigned char *buf;
    unsigned long pos;
    unsigned long size;
} out_desc_t;

/* in_func: provide pointer to available input and return number of bytes available */
static unsigned int my_in(void *in_desc_void, unsigned char **buf) {
    in_desc_t *d = (in_desc_t *)in_desc_void;
    *buf = (unsigned char *)d->buf;
    unsigned int ret = (unsigned int)(d->avail);
    d->buf += ret;
    d->avail -= ret;
    return ret;
}

/* out_func: copy output bytes into out_desc buffer and return 0 on success */
static int my_out(void *out_desc_void, unsigned char *buf, unsigned int len) {
    out_desc_t *d = (out_desc_t *)out_desc_void;
    memcpy(d->buf + d->pos, buf, len);
    d->pos += len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Prepare source data and compress into a buffer */
    const char src[] = "zlib API sequence payload: prepare -> compress -> write -> inflateBack -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    /* step 2: Initialize inflate stream and write compressed data to a gzip file */
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp, (unsigned int)compLen);

    /* step 3: Operate - set up stream buffers, call inflate, then invoke inflateBack and validate via adler32_combine */
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 16;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&strm, 0);

    unsigned char *window = (unsigned char *)malloc(1u << 15);
    memset(window, 0, 1u << 15);
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    in_desc_t in_desc;
    in_desc.buf = (const unsigned char *)comp;
    in_desc.avail = (unsigned long)compLen;

    out_desc_t out_desc;
    out_desc.buf = (unsigned char *)outbuf;
    out_desc.pos = 0;
    out_desc.size = (unsigned long)outBound;

    int rc_inflateBack = inflateBack(&strm, my_in, &in_desc, my_out, &out_desc);

    uLong len1 = (uLong)(strm.total_out / 2);
    uLong len2 = (uLong)(strm.total_out - len1);
    uLong sum1 = adler32(0L, outbuf, (uInt)len1);
    uLong sum2 = adler32(0L, outbuf + len1, (uInt)len2);
    uLong combined = adler32_combine(sum1, sum2, (off_t)len2);

    /* step 4: Cleanup - end inflate, end inflateBack, close gz file, free memory and finalize */
    int rc_inf_end = inflateEnd(&strm);
    int rc_back_end = inflateBackEnd(&strm);
    int rc_gzclose_r = gzclose_r(gzf);
    free(comp);
    free(outbuf);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_gzwrite;
    (void)rc_inflate;
    (void)rc_back_init;
    (void)rc_inflateBack;
    (void)sum1;
    (void)sum2;
    (void)combined;
    (void)rc_inf_end;
    (void)rc_back_end;
    (void)rc_gzclose_r;
    return 66;
}