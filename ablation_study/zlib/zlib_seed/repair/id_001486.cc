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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib api sequence: init deflate, compute bound, init inflate, compress and use gz64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    unsigned int read_buf_size = (unsigned int)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    uLong crc_read = crc32(0UL, read_buf, (uInt)(read_ret > 0 ? (unsigned int)read_ret : 0u));
    int gr_close_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)inf_init_ret;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)read_ret;
    (void)crc_read;
    (void)gr_close_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}