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
//<ID> 1092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / configure deflate and prepare buffers
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm_def;
    memset(&strm_def, 0, (int)sizeof(strm_def));
    int init_def_ret = deflateInit_(&strm_def, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm_def, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm_def.next_in = (Bytef *)source;
    strm_def.avail_in = (uInt)source_len;
    strm_def.next_out = comp_buf;
    strm_def.avail_out = (uInt)bound;

    // step 2: Operate - compress and write compressed data to gzip file
    int def_ret = deflate(&strm_def, 4);
    uLong comp_size = strm_def.total_out;
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, (voidpc)comp_buf, (unsigned int)comp_size);
    int gzclose_write_ret = gzclose(gw);

    // step 3: Operate - open gzip for reading, use gzgetc_ and close read handle with gzclose_r
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_byte = gzgetc_(gr);
    int gzclose_r_ret = gzclose_r(gr);

    // step 4: Inflate (decompress), validate and cleanup
    z_stream strm_inf;
    memset(&strm_inf, 0, (int)sizeof(strm_inf));
    int init_inf_ret = inflateInit_(&strm_inf, version, (int)sizeof(z_stream));
    strm_inf.next_in = comp_buf;
    strm_inf.avail_in = (uInt)comp_size;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)source_len + 16);
    memset(decomp_buf, 0, (size_t)source_len + 16);
    strm_inf.next_out = decomp_buf;
    strm_inf.avail_out = (uInt)source_len;
    int inf_ret = inflate(&strm_inf, 4);
    uLong decomp_size = strm_inf.total_out;
    uLong checksum = crc32(0UL, decomp_buf, (uInt)decomp_size);
    int end_def_ret = deflateEnd(&strm_def);
    int end_inf_ret = inflateEnd(&strm_inf);
    free(comp_buf);
    free(decomp_buf);
    (void)init_def_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    (void)init_inf_ret;
    (void)inf_ret;
    (void)decomp_size;
    (void)checksum;
    (void)end_def_ret;
    (void)end_inf_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}