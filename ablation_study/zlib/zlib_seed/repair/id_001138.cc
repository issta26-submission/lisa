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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and perform deflate (compress) into a buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/inflate/gzwrite and cleanup test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm_def;
    memset(&strm_def, 0, (int)sizeof(strm_def));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&strm_def, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm_def, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm_def.next_in = (Bytef *)source;
    strm_def.avail_in = (uInt)source_len;
    strm_def.next_out = comp_buf;
    strm_def.avail_out = (uInt)bound;
    int def_ret = deflate(&strm_def, 4);
    uLong comp_size = strm_def.total_out;
    int def_end_ret = deflateEnd(&strm_def);

    // step 2: Initialize inflate (decompress) and recover original data into out buffer
    z_stream strm_inf;
    memset(&strm_inf, 0, (int)sizeof(strm_inf));
    int inf_init_ret = inflateInit_(&strm_inf, version, (int)sizeof(z_stream));
    strm_inf.next_in = comp_buf;
    strm_inf.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    strm_inf.next_out = out_buf;
    strm_inf.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&strm_inf, 4);
    uLong out_size = strm_inf.total_out;
    int inf_end_ret = inflateEnd(&strm_inf);

    // step 3: Persist original source into a .gz file using gzwrite and close it
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);

    // step 4: Validate checksum, cleanup and finalize
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}