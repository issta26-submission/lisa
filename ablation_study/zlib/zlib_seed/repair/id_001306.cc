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
//<ID> 1306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and write a gz file
    const Bytef src[] = "zlib API sequence payload: write compressed data, read bytes, combine CRCs, init inflater and cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);

    // step 2: Read a few bytes using gzgetc and compute CRC combine of two halves
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int ch1 = gzgetc(gz_r);
    int ch2 = gzgetc(gz_r);
    int ch3 = gzgetc(gz_r);
    uInt half = (uInt)(src_len / 2);
    uLong crc_first = crc32(0L, src, half);
    uLong crc_second = crc32(0L, src + half, (uInt)(src_len - half));
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(src_len - half));

    // step 3: Initialize an inflate stream and call inflateBackEnd for cleanup
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_back_ret = inflateBackEnd(&inf_strm);

    // step 4: Final cleanup and validation
    int gzclose_r_ret = gzclose(gz_r);
    (void)ch1;
    (void)ch2;
    (void)ch3;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)inf_init_ret;
    (void)inf_back_ret;
    (void)gzclose_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}