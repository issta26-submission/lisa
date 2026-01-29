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
//<ID> 1485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data and initialize inflate stream, compute deflate bound)
    const Bytef src[] = "zlib api sequence: open with gzopen64, seek with gzseek64, use deflateBound and inflateInit_";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int infl_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);

    // step 2: Configure (write data into a gzip file using gzopen64, then seek and close)
    Bytef * write_buf = (Bytef *)malloc((size_t)src_len);
    memcpy(write_buf, src, (size_t)src_len);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)write_buf, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    off64_t pos_w = gzseek64(gw, (off64_t)0, 0);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (reopen with gzopen64 for reading, seek, read into a buffer sized by deflateBound, compute crc)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t pos_r = gzseek64(gr, (off64_t)0, 0);
    unsigned int read_buf_size = (unsigned int)((bound >= src_len) ? bound : src_len);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    uLong computed_crc = crc32(0UL, read_buf, (uInt)((read_ret > 0) ? (unsigned int)read_ret : 0u));
    int gr_close_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup (end inflate, free buffers, and tidy up)
    int infl_end_ret = inflateEnd(&istrm);
    free(write_buf);
    free(read_buf);
    (void)version;
    (void)infl_init_ret;
    (void)bound;
    (void)gw_write_ret;
    (void)pos_w;
    (void)gw_close_ret;
    (void)pos_r;
    (void)read_ret;
    (void)computed_crc;
    (void)gr_close_ret;
    (void)infl_end_ret;
    // API sequence test completed successfully
    return 66;
}