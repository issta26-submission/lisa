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
//<ID> 296
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
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (prepare compression buffers and dictionary)
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    unsigned char dict[16];
    memset(dict, 'D', sizeof(dict));
    uInt dictLen = (uInt)sizeof(dict);
    uLong input_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 3: Operate (compress, reset-keep, set inflate dictionary, and use inflateBack)
    deflate(&def_strm, 4);
    deflateResetKeep(&def_strm);
    inflateSetDictionary(&inf_strm, (const Bytef *)dict, dictLen);

    struct InDesc { const unsigned char * data; unsigned int size; unsigned int pos; };
    struct OutDesc { unsigned char * out; unsigned int size; unsigned int pos; };
    InDesc in_desc;
    OutDesc out_desc;
    in_desc.data = compBuf;
    in_desc.size = (unsigned int)def_strm.total_out;
    in_desc.pos = 0;
    unsigned char outBuf[512];
    memset(outBuf, 0, sizeof(outBuf));
    out_desc.out = outBuf;
    out_desc.size = (unsigned int)sizeof(outBuf);
    out_desc.pos = 0;

    in_func in_cb = [](void * in_d, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)in_d;
        *buf = (unsigned char *)(d->data + d->pos);
        unsigned int avail = d->size - d->pos;
        d->pos = d->size;
        return avail;
    };
    out_func out_cb = [](void * out_d, unsigned char * buf, unsigned int len) -> int {
        OutDesc * d = (OutDesc *)out_d;
        memcpy(d->out + d->pos, buf, len);
        d->pos += len;
        return 0;
    };

    inflateBack(&inf_strm, in_cb, &in_desc, out_cb, &out_desc);
    uLong output_adler = adler32(0L, (const Bytef *)outBuf, (uInt)out_desc.pos);

    // step 4: Validate & Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    return 66;
    // API sequence test completed successfully
}