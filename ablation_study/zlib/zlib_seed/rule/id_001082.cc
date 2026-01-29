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
//<ID> 1082
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'X', (size_t)SRC_LEN);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    Bytef *out = (Bytef *)malloc((size_t)SRC_LEN);
    memset(out, 0, (size_t)SRC_LEN);

    // step 2: Initialize deflate, compress data, finalize deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate (standard), validate, and exercise inflateBack path
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = out;
    istrm.avail_out = (uInt)SRC_LEN;
    inflate(&istrm, 0);
    inflateValidate(&istrm, 1);
    inflateEnd(&istrm);

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; uLong size; uLong pos; };
    struct OutDesc { unsigned char *data; uLong size; uLong pos; };
    InDesc in_desc;
    in_desc.data = comp;
    in_desc.size = comp_size;
    in_desc.pos = 0;
    OutDesc out_desc;
    out_desc.data = out;
    out_desc.size = SRC_LEN;
    out_desc.pos = 0;

    in_func in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int rem = (unsigned int)(desc->size - desc->pos);
        *buf = (unsigned char *)(desc->data + desc->pos);
        desc->pos += rem;
        return rem;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        unsigned int to_copy = len;
        memcpy(od->data + od->pos, buf, (size_t)to_copy);
        od->pos += to_copy;
        return 0;
    };

    inflateBack(&bstrm, in_cb, &in_desc, out_cb, &out_desc);
    inflateBackEnd(&bstrm);
    free(window);

    // step 4: Cleanup
    free(src);
    free(comp);
    free(out);
    (void)ver;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}