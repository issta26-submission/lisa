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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib api sequence demonstrating deflateInit_/deflatePrime/deflate/gzungetc/gzclose_r usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate stream and allocate output buffer
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound ? bound : 1));
    memset(comp_buf, 0, (int)(bound ? bound : 1));
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)((bound > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)bound);
    int prime_ret = deflatePrime(&strm, 3, 5);

    // step 3: Operate: perform deflate and finalize stream
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 4: Persist compressed data, exercise gz reading APIs, and cleanup
    gzFile gw = gzopen("zlib_test_tmp.gz", "wb");
    int gw_write_ret = 0;
    int gw_close_ret = 0;
    if (gw) {
        gw_write_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
        gw_close_ret = gzclose(gw);
    } else {
        gw_write_ret = -1;
        gw_close_ret = -1;
    }
    gzFile gr = gzopen("zlib_test_tmp.gz", "rb");
    int gu_ret = 0;
    int gr_close_r_ret = 0;
    if (gr) {
        gu_ret = gzungetc('Z', gr);
        gr_close_r_ret = gzclose_r(gr);
    } else {
        gu_ret = -1;
        gr_close_r_ret = -1;
    }
    int remove_ret = remove("zlib_test_tmp.gz");
    free(comp_buf);

    (void)version;
    (void)init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_size;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gu_ret;
    (void)gr_close_r_ret;
    (void)remove_ret;
    // API sequence test completed successfully
    return 66;
}