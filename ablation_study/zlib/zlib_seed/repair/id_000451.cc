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
//<ID> 451
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
    memset(&strm, 0, (int)sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef src_buf[128];
    memset(src_buf, 'A', (int)sizeof(src_buf));
    uLong src_len = (uLong)sizeof(src_buf);
    uLong bound = compressBound(src_len);
    Bytef *dest_buf = (Bytef *)malloc((size_t)bound);
    uLongf dest_len = (uLongf)bound;
    int compress_ret = compress(dest_buf, &dest_len, src_buf, src_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, dest_buf, (unsigned int)dest_len);
    int gzclose_ret1 = gzclose(gz);
    gzFile gz_in = gzopen(filename, "rb");
    Bytef read_buf[512];
    memset(read_buf, 0, (int)sizeof(read_buf));
    int gzread_ret = gzread(gz_in, read_buf, (unsigned int)dest_len);
    int gzclose_ret2 = gzclose(gz_in);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc = crc_table[0];
    int deflate_end_ret = deflateEnd(&strm);
    free(dest_buf);

    (void)ver;
    (void)deflate_init_ret;
    (void)compress_ret;
    (void)bound;
    (void)gzwrite_ret;
    (void)gzclose_ret1;
    (void)gzread_ret;
    (void)gzclose_ret2;
    (void)first_crc;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}