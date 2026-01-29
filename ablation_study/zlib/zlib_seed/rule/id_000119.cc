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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_stream;
    z_stream inf_stream;
    memset(&def_stream, 0, sizeof(def_stream));
    memset(&inf_stream, 0, sizeof(inf_stream));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (deflate init and compress)
    deflateInit2_(&def_stream, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    compress(comp, &compLen, (const Bytef *)input, inputLen);

    // step 3: Configure callbacks and operate (inflateBack + gzputs)
    struct InDesc { const unsigned char *data; unsigned int size; unsigned int pos; };
    struct OutDesc { unsigned char *out; unsigned int pos; };
    InDesc in_desc;
    OutDesc out_desc;
    in_desc.data = comp;
    in_desc.size = (unsigned int)compLen;
    in_desc.pos = 0;
    out_desc.out = output;
    out_desc.pos = 0;

    in_func in_cb = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int remain = id->size - id->pos;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += remain;
        return remain;
    };

    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->out + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };

    inflateBack(&inf_stream, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "zlib test sequence");
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_stream);
    free(comp);

    // API sequence test completed successfully
    return 66;
}