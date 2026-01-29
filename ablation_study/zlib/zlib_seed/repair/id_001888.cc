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
//<ID> 1888
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzwrite/gzread -> inflate -> inflateSync -> gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *ver = zlibVersion();
    int dinit = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and allocate buffers
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;

    // step 3: Operate - write compressed data to gz, read it back, then inflate and sync, check EOF
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gzclose_w = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)(comp_len > 0 ? (size_t)comp_len : 1U));
    memset(comp_buf2, 0, (int)(comp_len > 0 ? (int)comp_len : 1));
    int gr = gzread(gzr, comp_buf2, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_buf2;
    istrm.avail_in = (uInt)(comp_len > 0 ? (uInt)comp_len : 0U);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 4);
    int sync_ret = inflateSync(&istrm);
    int eof = gzeof(gzr);

    // step 4: Validate & Cleanup
    int dend = deflateEnd(&defstrm);
    int iend = inflateEnd(&istrm);
    int gzclose_r = gzclose(gzr);
    free(comp_buf);
    free(comp_buf2);
    free(out_buf);
    (void)dinit;
    (void)def_ret;
    (void)comp_len;
    (void)gw;
    (void)gzclose_w;
    (void)gr;
    (void)iinit;
    (void)inf_ret;
    (void)sync_ret;
    (void)eof;
    (void)dend;
    (void)iend;
    (void)gzclose_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}