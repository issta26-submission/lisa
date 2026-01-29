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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data, allocate buffers and perform a high-level compression
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflateInit_/deflateEnd/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, source, source_len);

    // step 2: Initialize a deflate stream (to be cleaned up later) and an inflate stream for decompression
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 3: Operate — feed compressed data into inflate and produce output, compute checksum for basic validation
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)comp_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = adler32(0UL, out_buf, (uInt)(out_size > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)out_size));

    // step 4: Cleanup — finalize streams and free resources
    int inf_back_ret = inflateBackEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);

    (void)comp_ret;
    (void)comp_len;
    (void)bound;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)checksum;
    (void)out_size;
    (void)version;
    (void)inf_back_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    return 66;
    // API sequence test completed successfully
}