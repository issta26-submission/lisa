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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source, compress, compute CRC)
    const Bytef src[] = "Zlib API sequence test: compute crc, compress, inflate, write with gzopen64, check eof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_destLen, src, src_len, 6);
    uLong comp_len = (uLong)comp_destLen;

    // step 2: Configure (initialize inflate stream and output buffer)
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

    // step 3: Operate (mark, inflate, translate error, write to gzip via gzopen64, check eof)
    long mark_before = inflateMark(&istrm);
    int inf_ret = inflate(&istrm, 0);
    const char *errstr = zError(inf_ret);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, decomp_buf, (unsigned int)(istrm.total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)istrm.total_out));
    int is_eof = gzeof(gz);
    int gzclose_ret = gzclose(gz);
    uLong crc_decomp = crc32(0UL, decomp_buf, (uInt)(istrm.total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)istrm.total_out));

    // step 4: Validate & Cleanup (end inflate, free buffers, neutralize unused vars)
    int inf_end_ret = inflateEnd(&istrm);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)mark_before;
    (void)errstr;
    (void)gw;
    (void)is_eof;
    (void)gzclose_ret;
    (void)crc_src;
    (void)crc_decomp;
    (void)version;
    (void)inf_ret;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}