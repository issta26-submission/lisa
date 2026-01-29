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
//<ID> 1883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzwrite -> gzeof -> inflateSync -> inflateEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream cstrm;
    memset(&cstrm, 0, (int)sizeof(cstrm));
    const char *ver = zlibVersion();
    int cinit = deflateInit_(&cstrm, 6, ver, (int)sizeof(z_stream));
    cstrm.next_in = (Bytef *)src;
    cstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&cstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    cstrm.next_out = comp_buf;
    cstrm.avail_out = (uInt)bound;

    // step 2: Operate - compress data using deflate
    int dret = deflate(&cstrm, 4);
    uLong comp_len = cstrm.total_out;

    // step 3: Operate - write compressed data to a gz file and check EOF
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gclose_w = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int is_eof = gzeof(gzr);
    int gclose_r = gzclose(gzr);

    // step 4: Configure inflater, inflate, sync, and cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0 ? (uInt)comp_len : 0U);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int iret = inflate(&istrm, 4);
    int sync_ret = inflateSync(&istrm);
    int iend = inflateEnd(&istrm);
    int dend = deflateEnd(&cstrm);
    free(comp_buf);
    free(out_buf);
    (void)cinit;
    (void)dret;
    (void)comp_len;
    (void)gw;
    (void)gclose_w;
    (void)is_eof;
    (void)gclose_r;
    (void)iinit;
    (void)iret;
    (void)sync_ret;
    (void)iend;
    (void)dend;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}