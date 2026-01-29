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
//<ID> 111
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
    unsigned char input[] = "Hello zlib world!";
    unsigned char outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    uLong inputLen = (uLong)(sizeof(input) - 1);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress((Bytef *)comp, &compLen, (const Bytef *)input, (uLong)inputLen);

    // step 2: Configure (deflate with extended init)
    unsigned char comp2_space[512];
    memset(comp2_space, 0, sizeof(comp2_space));
    deflateInit2_(&def, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = (Bytef *)comp2_space;
    def.avail_out = (uInt)sizeof(comp2_space);
    deflate(&def, 0);

    // step 3: Core operations (inflateBack using callbacks)
    memset(&inf, 0, sizeof(inf));
    inflateBackInit_(&inf, 15, (unsigned char *)NULL, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *data; unsigned int len; unsigned int pos; } in_desc;
    struct OutDesc { unsigned char *ptr; unsigned int pos; } out_desc;
    in_desc.data = comp;
    in_desc.len = (unsigned int)compLen;
    in_desc.pos = 0;
    out_desc.ptr = outbuf;
    out_desc.pos = 0;
    in_func in_cb = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *pd = (InDesc *)d;
        *buf = pd->data + pd->pos;
        unsigned int avail = pd->len - pd->pos;
        pd->pos = pd->len;
        return avail;
    };
    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *pd = (OutDesc *)d;
        memcpy(pd->ptr + pd->pos, buf, (size_t)len);
        pd->pos += len;
        return 0;
    };
    inflateBack(&inf, in_cb, &in_desc, out_cb, &out_desc);

    // step 4: Validate & Cleanup
    outbuf[out_desc.pos < sizeof(outbuf) ? out_desc.pos : (sizeof(outbuf) - 1)] = '\0';
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)outbuf);
    deflateEnd(&def);
    inflateBackEnd(&inf);
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}