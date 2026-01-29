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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

typedef struct {
    const Bytef *data;
    uLong len;
    uLong pos;
} in_desc_t;

/* in_func: supply the entire compressed buffer on the first call, then 0 */
static unsigned int in_func_impl(void *in_desc, unsigned char **buf) {
    in_desc_t *d = (in_desc_t *)in_desc;
    if (d->pos >= d->len) {
        *buf = NULL;
        return 0;
    }
    *buf = (unsigned char *)(d->data + d->pos);
    unsigned int ret = (unsigned int)(d->len - d->pos);
    d->pos = d->len;
    return ret;
}

/* out_func: accept output from inflateBack; do nothing and return success */
static int out_func_impl(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream cstrm;
    z_stream istrm;
    memset(&cstrm, 0, sizeof(cstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&cstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    /* step 2: Configure & Operate (compress) */
    uLong bound = deflateBound(&cstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    cstrm.next_in = (Bytef *)input;
    cstrm.avail_in = (uInt)inputLen;
    cstrm.next_out = compBuf;
    cstrm.avail_out = (uInt)bound;
    deflate(&cstrm, Z_FINISH);
    uLong compLen = cstrm.total_out;
    deflateEnd(&cstrm);

    /* step 3: Initialize inflateBack, use inflateUndermine & inflateBack */
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    in_desc_t ind;
    ind.data = compBuf;
    ind.len = compLen;
    ind.pos = 0;
    inflateBack(&istrm, (in_func)in_func_impl, (void *)&ind, (out_func)out_func_impl, (void *)window);
    inflateBackEnd(&istrm);

    /* step 4: Write compressed data to gz file and Cleanup */
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    return 66;
}