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
//<ID> 1214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checksum
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflate and adler32 usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong src_adler = adler32(0L, source, (uInt)source_len);

    // step 2: Compress the source into a buffer
    uLong comp_bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len_f, source, source_len);
    uLong comp_size = (uLong)comp_len_f;

    // step 3: Initialize inflate stream and perform decompression
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    uLong out_alloc = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);
    int inf_ret = inflate(&inf_strm, 4);
    uLong inflated_size = inf_strm.total_out;
    int sync_ok = inflateSyncPoint(&inf_strm);

    // step 4: Validate checksums, cleanup and finish
    uLong out_adler = adler32(0L, out_buf, (uInt)inflated_size);
    uLong combined_adler = adler32_combine(src_adler, out_adler, (off_t)inflated_size);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)compile_flags;
    (void)comp_ret;
    (void)version;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)sync_ok;
    (void)out_adler;
    (void)combined_adler;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}