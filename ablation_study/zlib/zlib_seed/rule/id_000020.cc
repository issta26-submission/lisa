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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/*Here's a corrected version that preserves the original straight-line logic (compress data, initialize inflateBack, write compressed data to a gzFile, use inflateBack with proper in/out callbacks, finish and cleanup). I added small helper callback implementations with signatures matching inflateBack's expected callbacks. The test function itself remains a straight-line sequence of API calls (no if/loops).

*/

/* Helper callback types and implementations for inflateBack */
typedef unsigned int (*in_func_t)(void *in_desc, unsigned char **buf);
typedef int (*out_func_t)(void *out_desc, unsigned char *buf, unsigned int len);

typedef struct {
    unsigned char *ptr;
    uLong left;
} my_in_desc_t;

static unsigned int my_in_func(void *in_desc, unsigned char **buf) {
    my_in_desc_t *d = (my_in_desc_t *)in_desc;
    if (d->left == 0) {
        return 0;
    }
    *buf = d->ptr;
    unsigned int avail = (unsigned int)d->left;
    d->left = 0;
    return avail;
}

static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    gzFile f = (gzFile)out_desc;
    int wrote = gzwrite(f, buf, len);
    return wrote;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream back_stream;
    unsigned char window_buf[1];
    unsigned char input_buf[128];
    uLong sourceLen = (uLong)sizeof(input_buf);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp_buf = (unsigned char *)malloc((size_t)cb);
    uLongf comp_len = (uLongf)cb;
    gzFile gz = 0;
    my_in_desc_t in_desc;

    memset(&back_stream, 0, sizeof(back_stream));
    memset(window_buf, 0, sizeof(window_buf));
    memset(input_buf, 'A', sizeof(input_buf));
    compress(comp_buf, &comp_len, input_buf, sourceLen);

    /* step 2: Configure inflateBack */
    inflateBackInit_(&back_stream, 15, window_buf, zlibVersion(), (int)sizeof(z_stream));

    /* step 3: Operate & Validate */
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp_buf, (unsigned int)comp_len);
    gzflush(gz, Z_FINISH); /* ensure data written out */

    in_desc.ptr = comp_buf;
    in_desc.left = (uLong)comp_len;

    inflateBack(&back_stream,
                (in_func_t)my_in_func,
                (void *)&in_desc,
                (out_func_t)my_out_func,
                (void *)gz);

    inflateBackEnd(&back_stream);

    /* step 4: Cleanup */
    free(comp_buf);
    gzclose(gz);

    /* API sequence test completed successfully */
    return 66;
}