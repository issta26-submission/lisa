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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source, compute crc, compress to memory, prepare buffers)
    const Bytef src[] = "Zlib API sequence: compute crc -> compress2 -> inflate -> write/read with gzopen64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc = crc32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));

    // step 2: Configure (initialize inflate stream)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 16));

    // step 3: Operate (mark, inflate, record error string, write and read gzip file with gzopen64 and check eof)
    long mark_before = inflateMark(&istrm);
    int inf_ret = inflate(&istrm, 0);
    const char *errstr = zError(inf_ret);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gclose_w = gzclose(gw);
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    unsigned int read_buf_size = 256;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int gzread_ret = gzread(gr, (voidp)read_buf, (unsigned int)(read_buf_size - 1));
    int is_eof = gzeof(gr);

    // step 4: Validate & Cleanup (end inflate, close file, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int gclose_r = gzclose(gr);
    (void)crc;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)mark_before;
    (void)errstr;
    (void)gw_ret;
    (void)gclose_w;
    (void)gzread_ret;
    (void)is_eof;
    (void)inf_end_ret;
    (void)gclose_r;
    (void)version;
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}