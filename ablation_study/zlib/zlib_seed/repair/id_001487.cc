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
//<ID> 1487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and streams)
    const Bytef src[] = "zlib sequence: compute bound, compress, write via gzopen64, seek64, read back, compute crc, cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure (determine bound and compress into buffer)
    uLong bound = deflateBound(&dstrm, src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Operate (write compressed data to a gz file using gzopen64, then read it back and seek)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (voidp)comp_buf, (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    unsigned int read_buf_size = (unsigned int)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (voidp)read_buf, read_buf_size);

    // step 4: Validate & Cleanup (compute CRCs, close files, free buffers, end inflate)
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)(read_ret > 0 ? (unsigned int)read_ret : 0u));
    int gr_close_ret = gzclose_r(gr);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)def_end_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)read_ret;
    (void)crc_src;
    (void)crc_read;
    (void)gr_close_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}