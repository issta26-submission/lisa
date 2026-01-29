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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream c_stream;
    z_stream i_stream;
    z_stream i_copy;
    memset(&c_stream, 0, sizeof(c_stream));
    memset(&i_stream, 0, sizeof(i_stream));
    memset(&i_copy, 0, sizeof(i_copy));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong compBound = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compBound);
    unsigned char *decomp = (unsigned char *)malloc((size_t)sourceLen * 4);
    memset(comp, 0, (size_t)compBound);
    memset(decomp, 0, (size_t)sourceLen * 4);
    inflateInit_(&i_stream, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&c_stream, 6, zlibVersion(), (int)sizeof(z_stream));
    c_stream.next_in = input;
    c_stream.avail_in = (uInt)sourceLen;
    c_stream.next_out = comp;
    c_stream.avail_out = (uInt)compBound;
    deflate(&c_stream, 0);

    // step 2: Configure
    deflateResetKeep(&c_stream);
    inflateCopy(&i_copy, &i_stream);
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    inflateBackInit_(&i_stream, 15, window, zlibVersion(), (int)sizeof(z_stream));

    struct InState { unsigned char *data; unsigned int len; unsigned int pos; };
    struct OutState { unsigned char *buf; unsigned int size; unsigned int pos; };
    InState inst;
    inst.data = comp;
    inst.len = (unsigned int)c_stream.total_out;
    inst.pos = 0;
    OutState outst;
    outst.buf = decomp;
    outst.size = (unsigned int)sourceLen * 4;
    outst.pos = 0;

    in_func in_cb = [](void *in_desc, unsigned char **buf) -> unsigned int {
        InState *s = (InState *)in_desc;
        unsigned int avail = s->len - s->pos;
        *buf = s->data + s->pos;
        s->pos = s->len;
        return avail;
    };
    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        OutState *o = (OutState *)out_desc;
        memcpy(o->buf + o->pos, buf, (size_t)len);
        o->pos += len;
        return 0;
    };

    // step 3: Operate & Validate
    inflateBack(&i_stream, in_cb, (void *)&inst, out_cb, (void *)&outst);
    inflateBackEnd(&i_stream);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outst.buf, (unsigned int)outst.pos);
    gzflush(gz, 0);

    // step 4: Cleanup
    deflateEnd(&c_stream);
    inflateEnd(&i_copy);
    inflateEnd(&i_stream);
    gzclose(gz);
    free(comp);
    free(decomp);
    free(window);
    // API sequence test completed successfully
    return 66;
}