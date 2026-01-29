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
//<ID> 457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    Bytef src[128];
    memset(src, 'A', sizeof(src));
    uLong src_len = (uLong)sizeof(src);

    // step 2: Configure / Prepare buffers
    uLong bound = compressBound(src_len);
    Bytef dest[512];
    memset(dest, 0, sizeof(dest));
    uLongf destLen = (uLongf)sizeof(dest);
    int compress_ret = compress(dest, &destLen, src, src_len);

    // step 3: Operate with gz and CRC table
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, dest, (unsigned int)destLen);
    int gzclose_w_ret = gzclose(gzw);

    gzFile gzr = gzopen(filename, "rb");
    Bytef readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gzr, readbuf, (unsigned int)sizeof(readbuf));

    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc_entry = crc_table[0];

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    int gzclose_r_ret = gzclose(gzr);

    (void)ver;
    (void)deflate_init_ret;
    (void)bound;
    (void)compress_ret;
    (void)destLen;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)first_crc_entry;
    (void)deflate_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}