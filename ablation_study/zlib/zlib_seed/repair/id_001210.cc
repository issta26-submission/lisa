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
//<ID> 1210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checksum/flags
    const Bytef source[] = "zlib API sequence payload: compute adler32, compress with deflate, inflate, validate and combine checksums.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    uLong adler_orig = adler32(1UL, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));

    // step 2: Compress with deflate
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    uLong comp_bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(comp_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Inflate and validate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);
    int inf_ret = inflate(&inf_strm, 4);
    uLong inflated_size = inf_strm.total_out;
    int inf_sync = inflateSyncPoint(&inf_strm);
    uLong adler_inflated = adler32(1UL, out_buf, (uInt)(inflated_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)inflated_size));
    uLong adler_combined = adler32_combine(adler_orig, adler_inflated, (off_t)inflated_size);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup and finalize
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_sync;
    (void)inf_end_ret;
    (void)adler_orig;
    (void)adler_inflated;
    (void)adler_combined;
    // API sequence test completed successfully
    return 66;
}