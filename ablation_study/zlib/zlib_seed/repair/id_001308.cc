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
//<ID> 1308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: write a gz file, read a byte, compute CRCs and cleanup inflate state";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure & Operate (write gz)
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzw, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gzw);

    // step 3: Operate & Validate (read first byte, compute CRCs, combine, teardown inflate)
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc(gzr);
    unsigned char first_byte_uc = (unsigned char)first_byte;
    uLong crc_full = crc32(0L, src_data, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    uLong crc_first = crc32(0L, (const Bytef *)&first_byte_uc, (uInt)1);
    uLong crc_combined = crc32_combine(crc_full, crc_first, (off_t)1);
    int inf_back_ret = inflateBackEnd(&inf_strm);
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Cleanup and finalize
    (void)inf_init_ret;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)first_byte;
    (void)crc_full;
    (void)crc_first;
    (void)crc_combined;
    (void)inf_back_ret;
    (void)gzclose_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}