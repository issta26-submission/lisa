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
//<ID> 1289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input descriptor for inflateBack in_func */
typedef struct {
    const unsigned char *data;
    uLong size;
    uLong offset;
} my_in_desc;

/* in_func: provide pointer to next input buffer and return number of bytes available */
static unsigned int my_in_func(void *in_desc, unsigned char **buf) {
    my_in_desc *d = (my_in_desc *)in_desc;
    if (d->offset >= d->size) {
        *buf = NULL;
        return 0;
    }
    *buf = (unsigned char *)(d->data + d->offset);
    unsigned int avail = (unsigned int)(d->size - d->offset);
    d->offset = d->size;
    return avail;
}

/* out_func: copy output bytes to out_desc buffer and return 0 for success */
static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    unsigned char *dest = (unsigned char *)out_desc;
    memcpy(dest, buf, (size_t)len);
    return 0;
}

int test_zlib_api_sequence() {
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);

    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    gzFile gf64 = gzopen64("test_zlib_api_sequence_64.gz", "wb");
    gzwrite(gf64, src, (unsigned int)SRC_LEN);
    gzclose(gf64);

    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, src, (unsigned int)SRC_LEN);
    gzclose(gf);

    uLongf decomp_len = SRC_LEN + 128;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp, 0, (size_t)decomp_len);
    uLong comp_size_uLong = (uLong)comp_len;
    uncompress2(decomp, &decomp_len, comp, &comp_size_uLong);

    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;

    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    my_in_desc *ind = (my_in_desc *)malloc(sizeof(my_in_desc));
    ind->data = comp;
    ind->size = comp_len;
    ind->offset = 0;

    unsigned char *out_buffer = (unsigned char *)decomp;

    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBack(&backstrm, (in_func)my_in_func, (void *)ind, (out_func)my_out_func, (void *)out_buffer);
    inflateBackEnd(&backstrm);

    free(ind);
    free(window);
    free(src);
    free(comp);
    free(decomp);

    return 66;
}