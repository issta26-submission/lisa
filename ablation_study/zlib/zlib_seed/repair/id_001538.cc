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
//<ID> 1538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compute CRC, tune deflate, write with gzfwrite and seek with gzseek";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = 0UL;
    uLong crc_value = crc32(crc_initial, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    int tune_ret = deflateTune(&strm, 4, 16, 32, 64);
    strm.adler = crc_value;

    // step 3: Operate
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)src, (z_size_t)1, (z_size_t)src_len, gz);
    off_t new_pos = gzseek(gz, (off_t)0, 0);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    (void)crc_initial;
    (void)crc_value;
    (void)def_init_ret;
    (void)ver;
    (void)tune_ret;
    (void)written;
    (void)new_pos;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}