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
//<ID> 1302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef data[] = "zlib fuzz payload example";
    uLong data_len = (uLong)(sizeof(data) - 1);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure - write compressed data to a gz file
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, data, (unsigned int)(data_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)data_len));
    int gzclose_w_ret = gzclose(gz_w);

    // step 3: Operate - reopen for read, fetch a byte, compute CRCs and combine
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gz_r);
    uLong len1 = data_len / 2;
    uLong crc1 = crc32(0L, data, (uInt)(len1 > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)len1));
    uLong crc2 = crc32(0L, data + len1, (uInt)((data_len - len1) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(data_len - len1)));
    uLong combined = crc32_combine(crc1, crc2, (off_t)(data_len - len1));
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Cleanup and validate
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    (void)version;
    (void)inf_init_ret;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)crc1;
    (void)crc2;
    (void)combined;
    (void)gzclose_r_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}