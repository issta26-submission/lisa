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
//<ID> 1429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "Zlib API sequence: write with gzopen64 -> read -> crc32 -> inflateMark -> zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (prepare read buffer and initialize an inflate stream)
    unsigned int read_buf_size = 512;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate (open for reading with gzopen64, read data, compute crc, attach to stream and query mark and EOF)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    int gr_read = gzread(gr, (voidp)read_buf, (unsigned int)(read_buf_size - 1));
    uLong crc_read = crc32(0UL, read_buf, (uInt)(gr_read > 0 ? (unsigned int)gr_read : 0u));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(gr_read > 0 ? (unsigned int)gr_read : 0u);
    long mark_val = inflateMark(&istrm);
    int eof_flag = gzeof(gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate & Cleanup (end inflate, obtain error string, free resources)
    int inf_end_ret = inflateEnd(&istrm);
    const char *errstr = zError(inf_end_ret);
    (void)crc_src;
    (void)crc_read;
    (void)gw_ret;
    (void)gw_close_ret;
    (void)inf_init_ret;
    (void)mark_val;
    (void)eof_flag;
    (void)gr_read;
    (void)gr_close_ret;
    (void)version;
    (void)errstr;
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}