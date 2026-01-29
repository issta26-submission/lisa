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
//<ID> 1425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compress -> inflate -> write via gzopen64 -> crc32 verify";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLongf comp_buf_len = (uLongf)compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_buf_len);
    memset(comp_buf, 0, (int)comp_buf_len);
    int comp_ret = compress2(comp_buf, &comp_buf_len, src, src_len, 6);
    uLong comp_len = (uLong)comp_buf_len;

    // step 2: Configure
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 16));

    // step 3: Operate
    int inf_ret = inflate(&istrm, 0);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gz, decomp_buf, (unsigned int)(istrm.total_out > 0 ? (unsigned int)istrm.total_out : (unsigned int)src_len));
    int eof_flag = gzeof(gz);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    long mark = inflateMark(&istrm);
    const char *errstr = zError(inf_ret);
    uLong crc_src = crc32(0L, src, (uInt)src_len);
    uLong crc_decomp = crc32(0L, decomp_buf, (uInt)(istrm.total_out > 0 ? (uInt)istrm.total_out : (uInt)src_len));
    int inf_end_ret = inflateEnd(&istrm);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)gw_ret;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)mark;
    (void)errstr;
    (void)crc_src;
    (void)crc_decomp;
    (void)version;
    (void)inf_end_ret;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}