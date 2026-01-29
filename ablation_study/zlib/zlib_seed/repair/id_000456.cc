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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure / Prepare data
    Bytef source[128];
    memset(source, 'A', sizeof(source));
    uLong source_len = (uLong)sizeof(source);
    Bytef compressed[512];
    memset(compressed, 0, sizeof(compressed));
    uLongf compressed_len = (uLongf)sizeof(compressed);
    int compress_ret = compress(compressed, &compressed_len, source, source_len);

    // step 3: Operate with gzFile and reading back
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compressed, (unsigned int)compressed_len);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    unsigned char readbuf[1024];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gzr, (voidp)readbuf, (unsigned int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_table = get_crc_table();
    uLong table_first = (uLong)crc_table[0];
    uLong accum = (uLong)compress_ret + (uLong)deflate_init_ret + (uLong)gzwrite_ret + (uLong)gzread_ret + (uLong)gzclose_w_ret + (uLong)gzclose_r_ret + (uLong)compressed_len + table_first;
    int deflate_end_ret = deflateEnd(&strm);

    (void)accum;
    (void)ver;
    (void)source_len;
    (void)compressed_len;
    (void)compress_ret;
    (void)deflate_init_ret;
    (void)gzwrite_ret;
    (void)gzread_ret;
    (void)gzclose_w_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}