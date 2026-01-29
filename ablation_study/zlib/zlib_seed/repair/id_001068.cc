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
//<ID> 1068
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload: compress with compress2, inflate, write to gz, seek, and compute combined CRC.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong comp_bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLong comp_size = comp_bound;
    int comp_ret = compress2(comp_buf, (uLongf *)&comp_size, source, source_len, 6);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_size;
    uLong out_buf_len = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (size_t)out_buf_len);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_buf_len;
    int und_ret = inflateUndermine(&strm, 1);

    // step 3: Operate
    int inf_ret = inflate(&strm, 4);
    uLong out_size = strm.total_out;
    int inf_end_ret = inflateEnd(&strm);
    gzFile gzf = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, out_buf, (unsigned int)out_size);
    off_t seek_pos = gzseek(gzf, 0, 0);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong half = (uLong)(out_size / 2);
    uLong len2 = out_size - half;
    uLong crc1 = crc32(0UL, out_buf, (uInt)half);
    uLong crc2 = crc32(0UL, out_buf + half, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)und_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)seek_pos;
    (void)gzclose_ret;
    (void)combined_crc;
    (void)version;
    (void)source_len;
    (void)comp_bound;
    (void)comp_size;
    (void)out_size;
    // API sequence test completed successfully
    return 66;
}