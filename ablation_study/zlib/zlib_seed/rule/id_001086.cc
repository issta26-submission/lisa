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
//<ID> 1086
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize deflate, compress data, finalize deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, perform inflate, validate, then exercise inflateBack with simple in/out callbacks
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateValidate(&istrm, 1);
    in_func in_cb = static_cast<in_func>([](void *in_desc, unsigned char **buf) -> unsigned int {
        z_stream *s = (z_stream *)in_desc;
        *buf = s->next_in;
        unsigned int n = s->avail_in;
        s->next_in += n;
        s->avail_in = 0;
        return n;
    });
    out_func out_cb = static_cast<out_func>([](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        z_stream *s = (z_stream *)out_desc;
        memcpy(s->next_out, buf, (size_t)len);
        s->next_out += len;
        s->avail_out -= len;
        s->total_out += len;
        return (int)len;
    });
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateBack(&istrm, in_cb, &istrm, out_cb, &istrm);
    inflateEnd(&istrm);

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}