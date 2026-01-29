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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'Z', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize inflater and query compile flags)
    inflateInit2_(&strm, 15, zlibVersion(), (int)sizeof(z_stream));
    uLong flags = zlibCompileFlags();
    (void)flags;
    inflateBackInit_(&strm, 15, (unsigned char *)NULL, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (use inflateBack with memory-backed in/out callbacks)
    struct InDesc { unsigned char *data; uLong size; uLong pos; };
    struct OutDesc { unsigned char *data; uLong size; uLong pos; };
    InDesc ind = { compBuf, compLen, 0 };
    OutDesc outd = { output, (uLong)sizeof(output), 0 };

    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *h = (InDesc *)desc;
        unsigned int avail = (unsigned int)(h->size - h->pos);
        *buf = h->data + (size_t)h->pos;
        h->pos = h->size;
        return avail;
    };
    out_func out_cb = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->data + (size_t)o->pos, buf, (size_t)len);
        o->pos += (uLong)len;
        return 1;
    };

    inflateBack(&strm, in_cb, (void *)&ind, out_cb, (void *)&outd);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)outd.pos);
    (void)checksum;
    inflateBackEnd(&strm);
    inflateEnd(&strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}