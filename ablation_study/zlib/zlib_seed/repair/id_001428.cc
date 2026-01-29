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
//<ID> 1428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "Zlib API sequence test data: write with gzopen64 -> read -> inflate -> checksum";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0UL, src, (uInt)src_len);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, src, (unsigned int)(src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open for reading and load compressed data into a buffer)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    int eof_flag = gzeof(gr);
    uInt read_buf_size = (uInt)(src_len + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int gzread_ret = gzread(gr, (void *)read_buf, (unsigned int)(read_buf_size - 1));

    // step 3: Operate (initialize inflate, set input, inflate, get mark and error string, compute checksum)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)gzread_ret;
    uLong decomp_size = src_len + 128;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_size);
    memset(decomp_buf, 0, (int)decomp_size);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)decomp_size;
    int inf_ret = inflate(&istrm, 0);
    long mark = inflateMark(&istrm);
    const char *errstr = zError(inf_ret);
    uLong crc_decomp = crc32(0UL, decomp_buf, (uInt)istrm.total_out);

    // step 4: Validate & Cleanup (end inflate, close file, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int gclose_ret = gzclose(gr);
    (void)crc_initial;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)eof_flag;
    (void)gzread_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)mark;
    (void)errstr;
    (void)crc_decomp;
    (void)inf_end_ret;
    (void)gclose_ret;
    (void)version;
    free(read_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}