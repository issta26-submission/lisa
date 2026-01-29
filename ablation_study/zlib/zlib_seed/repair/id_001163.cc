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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and perform a deflate-based stream compression
    const Bytef source[] = "zlib API sequence payload demonstrating deflateEnd/compress/inflateInit_/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 2: Also compress the same source using the one-shot compress() API
    uLong comp2_bound = compressBound(source_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_bound);
    uLongf comp2_len = (uLongf)comp2_bound;
    memset(comp2_buf, 0, (size_t)comp2_bound);
    int comp_ret = compress(comp2_buf, &comp2_len, source, source_len);

    // step 3: Initialize an inflate stream, decompress the deflate-produced buffer and compute a checksum
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = crc32(0UL, out_buf, (uInt)(out_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_size));
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Initialize an inflate-back stream (inflateBackInit_) and then clean it up with inflateBackEnd; free resources
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);
    free(comp_buf);
    free(comp2_buf);
    free(out_buf);
    free(window);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)comp_ret;
    (void)comp2_len;
    (void)comp2_bound;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)checksum;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66; // API sequence test completed successfully
}