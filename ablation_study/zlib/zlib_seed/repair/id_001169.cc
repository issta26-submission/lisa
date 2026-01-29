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
//<ID> 1169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a dynamically allocated buffer
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflateInit_/inflateBackEnd/deflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong dest_alloc = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)dest_alloc);
    memset(comp_buf, 0, (size_t)dest_alloc);
    uLongf comp_size = (uLongf)dest_alloc;
    int comp_ret = compress(comp_buf, &comp_size, source, source_len);

    // step 2: Initialize inflate stream and allocate output buffer
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);

    // step 3: Configure inflate stream with compressed data and perform inflate operation
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = adler32(0UL, out_buf, (uInt)(out_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_size));

    // step 4: Cleanup streams and buffers, finalize with required API calls
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);

    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_size;
    (void)checksum;
    (void)inf_back_end_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    (void)source_len;
    (void)dest_alloc;
    // API sequence test completed successfully
    return 66;
}