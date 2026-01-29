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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptor for the input callback passed to inflateBack */
typedef struct {
    unsigned char *buf;
    unsigned int len;
} in_desc_t;

/* in_func: provide the whole buffer once */
static unsigned int test_in_func(void *in_desc, unsigned char **bufptr) {
    in_desc_t *d = (in_desc_t *)in_desc;
    *bufptr = d->buf;
    return d->len;
}

/* out_func: consume output without doing anything */
static int test_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    z_stream srcStrm;
    memset(&srcStrm, 0, sizeof(srcStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)128);
    memset(input, 'X', 128);
    uLong inputLen = (uLong)128;
    unsigned char *window = (unsigned char *)malloc(1);
    memset(window, 0, 1);

    /* prepare input descriptor for inflateBack */
    in_desc_t *idesc = (in_desc_t *)malloc(sizeof(in_desc_t));
    idesc->buf = (unsigned char *)input;
    idesc->len = (unsigned int)inputLen;

    /* step 2: Initialize streams and back-end */
    srcStrm.next_in = input;
    srcStrm.avail_in = (uInt)inputLen;
    inflateInit_(&srcStrm, ver, (int)sizeof(z_stream));
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    /* step 3: Operate: copy inflate state, run inflateBack with callbacks, write to gz file, query flags */
    inflateCopy(&copyStrm, &srcStrm);
    inflateBack(&backStrm, (in_func)test_in_func, (void *)idesc, (out_func)test_out_func, (void *)NULL);
    gzFile gz = gzopen("tmp_test_zlib_api.gz", "wb");
    gzputc(gz, 'Z');
    uLong flags = zlibCompileFlags();
    (void)flags;

    /* step 4: Cleanup */
    inflateEnd(&srcStrm);
    inflateEnd(&copyStrm);
    inflateBackEnd(&backStrm);
    gzclose(gz);
    free(input);
    free(window);
    free(idesc);

    return 66;
}