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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (compress + deflate init2)
    compress((Bytef *)comp, &compLen, (const Bytef *)input, inputLen);
    deflateInit2_(&def, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = (Bytef *)comp;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    unsigned int def_out_len = (unsigned int)def.total_out;

    // step 3: Core operations (inflateBack with callbacks -> collect output)
    struct InDesc { const unsigned char *data; unsigned int len; };
    struct OutDesc { unsigned char *buf; unsigned int pos; unsigned int size; };
    InDesc ind;
    ind.data = comp;
    ind.len = (unsigned int)compLen;
    OutDesc outd;
    outd.buf = output;
    outd.pos = 0;
    outd.size = (unsigned int)sizeof(output);
    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = (unsigned char *)d->data;
        return d->len;
    };
    out_func out_cb = [](void *desc, unsigned char *data, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        unsigned int copy_len = len;
        memcpy(o->buf + o->pos, data, copy_len);
        o->pos += copy_len;
        return 0;
    };
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inflateBack(&inf, in_cb, &ind, out_cb, &outd);
    unsigned int final_out_len = outd.pos;

    // step 4: Validate & Cleanup (write a small message via gzputs, close & free)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "inflateBack produced output\n");
    gzputs(gz, "Finished\n");
    gzclose(gz);
    deflateEnd(&def);
    inflateEnd(&inf);
    free(comp);

    // API sequence test completed successfully
    return 66;
}