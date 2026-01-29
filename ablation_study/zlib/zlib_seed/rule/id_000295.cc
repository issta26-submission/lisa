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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* helper input context for inflateBack */
typedef struct {
    const Bytef *buf;
    uLong len;
    uLong pos;
} my_in_ctx;
/* helper output context for inflateBack */
typedef struct {
    Bytef *buf;
    uLong len;
    uLong pos;
} my_out_ctx;

/* in_func for inflateBack: provide pointer to next available input chunk */
static unsigned int my_in_func(void *in_desc, unsigned char **bufptr) {
    my_in_ctx *c = (my_in_ctx *)in_desc;
    if (c->pos >= c->len) {
        *bufptr = NULL;
        return 0;
    }
    *bufptr = (unsigned char *)(c->buf + c->pos);
    unsigned int avail = (unsigned int)(c->len - c->pos);
    c->pos += avail;
    return avail;
}

/* out_func for inflateBack: copy output chunk into provided output buffer */
static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    my_out_ctx *c = (my_out_ctx *)out_desc;
    if ((uLong)len > (c->len - c->pos)) {
        /* truncate if necessary to avoid overflow */
        len = (unsigned int)(c->len - c->pos);
    }
    memcpy(c->buf + c->pos, buf, len);
    c->pos += len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    const char * ver = zlibVersion();
    deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    /* step 2: Configure */
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;

    /* step 3: Operate & Validate */
    deflate(&def_strm, Z_FINISH);
    deflateResetKeep(&def_strm);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);

    /* prepare contexts for inflateBack */
    my_in_ctx inctx;
    inctx.buf = compBuf;
    inctx.len = def_strm.total_out;
    inctx.pos = 0;
    my_out_ctx outctx;
    outctx.buf = (Bytef *)outBuf;
    outctx.len = sizeof(outBuf);
    outctx.pos = 0;

    inflateSetDictionary(&inf_strm, (const Bytef *)input, (uInt)8);
    inflateBack(&inf_strm, (in_func)my_in_func, (void *)&inctx, (out_func)my_out_func, (void *)&outctx);

    /* step 4: Cleanup */
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);
    return 66;
}