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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

struct in_desc {
    unsigned char *buf;
    unsigned int len;
    unsigned int pos;
};

struct out_desc {
    unsigned char *buf;
    unsigned int size;
    unsigned int pos;
};

/* in_func: provide a pointer to available input and return available length */
static unsigned int my_in(void *in_desc, unsigned char **buf) {
    struct in_desc *d = (struct in_desc *)in_desc;
    unsigned int rem = d->len - d->pos;
    *buf = d->buf + d->pos;
    d->pos += rem;
    return rem;
}

/* out_func: consume output data */
static int my_out(void *out_desc, unsigned char *buf, unsigned int len) {
    struct out_desc *o = (struct out_desc *)out_desc;
    memcpy(o->buf + o->pos, buf, len);
    o->pos += len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[64];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    const char * version = zlibVersion();
    adler32(0UL, input, (uInt)inputLen);
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    /* prepare inflate-back environment with a window and init */
    unsigned char window[(1 << 15)];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&inf_strm, 15, window, version, (int)sizeof(z_stream));

    /* step 2: Configure */
    Bytef * dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);
    inflateSetDictionary(&inf_strm, dict, (uInt)32);
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound + 1);
    memset(compBuf, 0, (size_t)compBound + 1);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)(compBound + 1);

    /* step 3: Operate */
    deflate(&def_strm, 4);
    deflateResetKeep(&def_strm);
    unsigned char outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));

    struct in_desc id;
    id.buf = compBuf;
    id.len = (unsigned int)def_strm.total_out;
    id.pos = 0;
    struct out_desc od;
    od.buf = outBuf;
    od.size = (unsigned int)sizeof(outBuf);
    od.pos = 0;

    inflateBack(&inf_strm, (in_func)my_in, &id, (out_func)my_out, &od);

    /* step 4: Cleanup */
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(dict);
    return 66;
}