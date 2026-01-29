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
//<ID> 1426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "Zlib API sequence: write gz via gzopen64 -> read -> crc32 -> inflate operations";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_wr = gzwrite(gw, (voidp)src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close = gzclose(gw);

    // step 2: Configure (open for reading, read contents, compute CRC, prepare inflate stream)
    unsigned int read_buf_size = (unsigned int)(src_len + 32);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    int gr_read = gzread(gr, (voidp)read_buf, (unsigned int)(read_buf_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : read_buf_size));
    uInt read_len = (uInt)(gr_read > 0 ? (unsigned int)gr_read : 0u);
    uLong computed_crc = crc32(0L, read_buf, read_len);

    // step 3: Operate (initialize inflate stream, attempt inflate on the read buffer, query inflateMark and gzeof)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = read_len;
    unsigned int out_buf_size = (unsigned int)(src_len + 64);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_size);
    memset(out_buf, 0, (int)out_buf_size);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(out_buf_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : out_buf_size);
    int inf_ret = inflate(&istrm, 0);
    long markpos = inflateMark(&istrm);
    int eof_flag = gzeof(gr);
    const char *errstr = zError(inf_ret);

    // step 4: Validate & Cleanup (end inflate stream, close file, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int gr_close = gzclose(gr);
    (void)gw_wr;
    (void)gw_close;
    (void)gr_read;
    (void)computed_crc;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)markpos;
    (void)eof_flag;
    (void)errstr;
    (void)inf_end_ret;
    (void)gr_close;
    (void)version;
    free(read_buf);
    free(out_buf);
    // API sequence test completed successfully
    return 66;
}