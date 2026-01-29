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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream inflatestrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&inflatestrm, 0, sizeof(inflatestrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress((Bytef *)comp, &compLen, (const Bytef *)input, inputLen);

    // step 2: Configure
    deflateInit2_(&defstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inflatestrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    struct InDesc { const unsigned char *data; unsigned int len; unsigned int used; };
    struct OutDesc { unsigned char *out; unsigned int capacity; unsigned int used; };
    InDesc *inDesc = (InDesc *)malloc(sizeof(InDesc));
    inDesc->data = comp;
    inDesc->len = (unsigned int)compLen;
    inDesc->used = 0;
    unsigned int outCapacity = (unsigned int)inputLen + 16;
    unsigned char *outbuf = (unsigned char *)malloc((size_t)outCapacity);
    memset(outbuf, 0, (size_t)outCapacity);
    OutDesc *outDesc = (OutDesc *)malloc(sizeof(OutDesc));
    outDesc->out = outbuf;
    outDesc->capacity = outCapacity;
    outDesc->used = 0;
    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int remaining = d->len - d->used;
        *buf = (unsigned char *)(d->data + d->used);
        d->used = d->len;
        return remaining;
    };
    out_func out_cb = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->out + o->used, buf, (size_t)len);
        o->used += len;
        return 0;
    };
    inflateBack(&inflatestrm, in_cb, (void *)inDesc, out_cb, (void *)outDesc);
    outbuf[outDesc->used] = '\0';
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)outbuf);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&inflatestrm);
    free(comp);
    free(inDesc);
    free(outbuf);
    free(outDesc);

    // API sequence test completed successfully
    return 66;
}