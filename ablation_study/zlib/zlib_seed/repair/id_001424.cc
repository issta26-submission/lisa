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
//<ID> 1424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data, compute CRC, write gzip file)
    const Bytef src[] = "zlib API sequence: write with gzopen64 -> read -> inflateMark -> crc32 -> zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    gzFile gzw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, src, (unsigned int)src_len);
    int gzw_close_ret = gzclose(gzw);

    // step 2: Configure (open for reading, read into buffer, compute CRC of read data, check EOF)
    gzFile gzr = gzopen64("test_zlib_api_sequence.gz", "rb");
    unsigned int read_buf_size = 512;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int gzread_ret = gzread(gzr, (voidp)read_buf, (unsigned int)read_buf_size);
    uLong crc_read = crc32(0UL, read_buf, (uInt)gzread_ret);
    int is_eof = gzeof(gzr);

    // step 3: Operate (initialize inflate stream, attach input, query mark, run inflate, obtain error string)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)gzread_ret;
    long mark = inflateMark(&istrm);
    int inf_ret = inflate(&istrm, 0);
    const char *errstr = zError(inf_ret);

    // step 4: Validate & Cleanup (end inflate, close file, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int gzr_close_ret = gzclose(gzr);
    free(read_buf);
    (void)crc_src;
    (void)gw_ret;
    (void)gzw_close_ret;
    (void)crc_read;
    (void)is_eof;
    (void)inf_init_ret;
    (void)mark;
    (void)errstr;
    (void)inf_end_ret;
    (void)gzr_close_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}