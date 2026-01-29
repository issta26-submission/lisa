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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "Example payload for zlib demonstrating deflate, deflateTune, gzopen and gzrewind usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (tune deflate and prepare output buffer)
    int tune_ret = deflateTune(&strm, 20, 10, 30, 5);
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate (perform compression)
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 4: Validate and cleanup (write compressed data to gzip file, rewind, close, free)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int to_write = (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size);
    int gz_write_ret = gzf ? gzwrite(gzf, comp_buf, to_write) : 0;
    int gz_rewind_ret = gzf ? gzrewind(gzf) : 0;
    int gz_close_ret = gzf ? gzclose(gzf) : 0;
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_rewind_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}