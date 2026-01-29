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
//<ID> 1482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflateBound, gzopen64/gzseek64, inflateInit_, compress2, crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure (compute bound, compress, write with gzopen64 and gzseek64)
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)comp_buf, (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len));
    off64_t gw_seek_ret = gzseek64(gw, 0, 0);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (reopen with gzopen64, seek with gzseek64, read and compute CRCs)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t gr_seek_ret = gzseek64(gr, 0, 0);
    unsigned int read_buf_size = (unsigned int)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)((read_ret > 0) ? (unsigned int)read_ret : 0u));

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int gr_close_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)comp_ret;
    (void)gw_write_ret;
    (void)gw_seek_ret;
    (void)gw_close_ret;
    (void)gr_seek_ret;
    (void)read_ret;
    (void)crc_src;
    (void)crc_read;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}