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
//<ID> 1420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test payload: compute crc32 -> compress2 -> inflate -> write gz (64-bit) -> check eof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;
    uLong crc_src = crc32(0L, src, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));

    // step 2: Configure
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_buf_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(out_buf_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_buf_len);

    // step 3: Operate
    int prime_ret = inflatePrime(&istrm, 1, 1);
    int inf_ret = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)(istrm.total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)istrm.total_out);
    int gw = gzwrite(gz, out_buf, write_len);
    int eof_flag = gzeof(gz);
    const char *err_str = zError(inf_ret);
    uLong crc_out = crc32(0L, out_buf, (uInt)write_len);

    // step 4: Validate & Cleanup
    (void)comp_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)mark;
    (void)gw;
    (void)eof_flag;
    (void)err_str;
    (void)crc_src;
    (void)crc_out;
    int inf_end_ret = inflateEnd(&istrm);
    int gzclose_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    // API sequence test completed successfully
    return 66;
}