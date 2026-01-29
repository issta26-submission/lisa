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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    const char *ver = zlibVersion();
    deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    uLong initial_adler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Configure (prepare compression buffers and dictionary)
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    Bytef dict[16];
    memset(dict, 'D', sizeof(dict));
    uInt dictLen = (uInt)sizeof(dict);
    uLong dict_adler = adler32(0UL, dict, dictLen);

    // step 3: Operate (compress, reset keep, set inflate dictionary, and inflateBack using callbacks)
    deflate(&def_strm, 4);
    deflateResetKeep(&def_strm);
    inflateSetDictionary(&inf_strm, dict, dictLen);

    struct InDesc { Bytef *buf; uInt len; uInt pos; };
    struct OutDesc { Bytef *buf; uInt cap; uInt pos; };
    InDesc inDesc;
    inDesc.buf = compBuf;
    inDesc.len = (uInt)def_strm.total_out;
    inDesc.pos = 0;
    Bytef outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    OutDesc outDesc;
    outDesc.buf = outBuf;
    outDesc.cap = (uInt)sizeof(outBuf);
    outDesc.pos = 0;

    auto in_lambda = +[](void *desc, unsigned char **buf)->unsigned int {
        InDesc *d = (InDesc*)desc;
        unsigned int chunk = d->len - d->pos;
        *buf = d->buf + d->pos;
        d->pos = d->len;
        return chunk;
    };
    auto out_lambda = +[](void *desc, unsigned char *buf, unsigned int len)->int {
        OutDesc *d = (OutDesc*)desc;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += len;
        return 0;
    };

    inflateBack(&inf_strm, (in_func)in_lambda, (void *)&inDesc, (out_func)out_lambda, (void *)&outDesc);

    // step 4: Validate & Cleanup
    uLong output_adler = adler32(0UL, outBuf, outDesc.pos);
    (void)initial_adler;
    (void)dict_adler;
    (void)output_adler;
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    return 66;
    // API sequence test completed successfully
}