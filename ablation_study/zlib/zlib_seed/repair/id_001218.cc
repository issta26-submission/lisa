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
//<ID> 1218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "Example payload for zlib API sequence demonstrating deflate/inflate and adler checks.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    uLong adler_src = adler32(0L, source, (uInt)source_len);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate and produce compressed data
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    uLong comp_bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate, decompress, validate sync and compute adler checks
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int sync_point = inflateSyncPoint(&inf_strm);
    uLong adler_out = adler32(0L, out_buf, (uInt)out_size);
    uLong combined_adler = adler32_combine(adler_src, adler_out, (off_t)out_size);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)sync_point;
    (void)adler_src;
    (void)adler_out;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}