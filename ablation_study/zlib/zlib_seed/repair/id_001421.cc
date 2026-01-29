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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compute original CRC)
    const Bytef src[] = "Zlib API sequence sample: compress -> inflate -> write gz -> validate with crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_orig = crc32(0L, src, (uInt)src_len);

    // step 2: Configure (compress source to a zlib-format buffer)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 3: Operate (inflate the compressed buffer into an output buffer and inspect stream)
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
    int inf_ret = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    const char *errstr = zError(inf_ret);
    uLong crc_after_inflate = crc32(0L, decomp_buf, (uInt)src_len);

    // step 4: Validate & Cleanup (write inflated data to a gzip file, check EOF flag, cleanup)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, decomp_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int eof_flag = gzeof(gz);
    int gzclose_ret = gzclose(gz);
    int inf_end_ret = inflateEnd(&istrm);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)mark;
    (void)errstr;
    (void)crc_orig;
    (void)crc_after_inflate;
    (void)gw;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)inf_end_ret;
    (void)version;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}