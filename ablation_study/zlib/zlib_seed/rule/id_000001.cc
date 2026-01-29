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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Minimal in/out callbacks for inflateBack.
   in_func: supplies a pointer to input buffer and returns number of bytes available (we return 0).
   out_func: consumes output bytes and returns 0.
*/
static unsigned int my_in(void *in_desc, unsigned char **buf) {
    *buf = (unsigned char *)in_desc;
    return 0;
}
static int my_out(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & zero-initialize structures and buffers (Initialize) */
    z_stream infl_src;
    z_stream infl_copy;
    z_stream def_strm;
    z_stream back_strm;
    memset(&infl_src, 0, sizeof(z_stream));
    memset(&infl_copy, 0, sizeof(z_stream));
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&back_strm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    unsigned char input_data[1];
    input_data[0] = 0;
    const char *ver = zlibVersion();

    /* step 2: Initialize and configure inflate/deflate/back streams (Configure) */
    inflateInit_(&infl_src, ver, (int)sizeof(z_stream));
    inflateCopy(&infl_copy, &infl_src);
    inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));
    deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ver, (int)sizeof(z_stream));

    /* step 3: Operate and validate core APIs (Operate → Validate) */
    deflateResetKeep(&def_strm);
    inflateBack(&back_strm, (in_func)my_in, input_data, (out_func)my_out, NULL);
    inflateBackEnd(&back_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gzf);
    gzclose(gzf);

    /* step 4: Cleanup */
    inflateEnd(&infl_src);
    inflateEnd(&infl_copy);
    deflateEnd(&def_strm);

    /* API sequence test completed successfully */
    return 66;
}