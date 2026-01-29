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
//<ID> 1422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data, compute checksums, compress)
    const Bytef src[] = "Zlib API sequence sample data for gzip64, crc, inflateMark and zError usage.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_len);

    // step 2: Configure (initialize inflate stream and output buffer)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);

    // step 3: Operate (inflate the compressed buffer, inspect mark and error, write output to gz via gzopen64)
    int inf_ret = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    const char *errstr = zError(inf_ret);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, decomp_buf, (unsigned int)istrm.total_out);
    int is_eof = gzeof(gz);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup (end inflate, free buffers, and tidy up)
    int inf_end_ret = inflateEnd(&istrm);
    (void)crc_src;
    (void)crc_comp;
    (void)version;
    (void)bound;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)mark;
    (void)errstr;
    (void)gw;
    (void)is_eof;
    (void)gzclose_ret;
    (void)inf_end_ret;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}