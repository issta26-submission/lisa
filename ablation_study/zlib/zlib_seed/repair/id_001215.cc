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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checks
    const Bytef source[] = "zlib API sequence payload demonstrating inflateInit_/inflateSyncPoint/adler32/adler32_combine usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    uLong ad_src = adler32(1UL, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));

    // step 2: Initialize and configure inflate stream
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = (Bytef *)source;
    inf_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);

    // step 3: Operate and validate stream state and checksums
    int sync_point = inflateSyncPoint(&inf_strm);
    uLong ad_out = adler32(0UL, out_buf, (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc));
    uLong combined_adler = adler32_combine(ad_src, ad_out, (off_t)source_len);

    // step 4: Cleanup and finalize
    int inf_end_ret = inflateEnd(&inf_strm);
    free(out_buf);
    (void)version;
    (void)compile_flags;
    (void)inf_init_ret;
    (void)sync_point;
    (void)ad_src;
    (void)ad_out;
    (void)combined_adler;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}