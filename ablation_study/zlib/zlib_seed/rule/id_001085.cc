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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* helper input function for inflateBack: provides the whole compressed buffer at once */
static unsigned int fuzz_in_func(void *in_desc, unsigned char **buf) {
    struct in_desc_s { unsigned char *ptr; unsigned int len; };
    struct in_desc_s *d = (struct in_desc_s *)in_desc;
    *buf = d->ptr;
    return d->len;
}

/* helper output function for inflateBack: copy output to the provided output buffer */
static int fuzz_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    unsigned char *dst = (unsigned char *)out_desc;
    memcpy(dst, buf, len);
    (void)dst;
    return 0;
}

int test_zlib_api_sequence() {
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

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
    deflate(&dstrm, 4); /* use numeric flush value as in original sequence */
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

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

    /* prepare descriptor for inflateBack and call it with valid function pointers */
    struct in_desc_s { unsigned char *ptr; unsigned int len; } in_desc;
    in_desc.ptr = comp;
    in_desc.len = (unsigned int)comp_size;
    inflateBack(&istrm, (in_func)fuzz_in_func, &in_desc, (out_func)fuzz_out_func, decomp);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    return 66;
}