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
//<ID> 1884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> read gz -> inflateSync -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    // step 2: Configure compressor and buffers
    z_stream cstrm;
    memset(&cstrm, 0, (int)sizeof(cstrm));
    int cinit = deflateInit_(&cstrm, 6, ver, (int)sizeof(z_stream));
    cstrm.next_in = (Bytef *)src;
    cstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&cstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    cstrm.next_out = comp_buf;
    cstrm.avail_out = (uInt)bound;
    int dret = deflate(&cstrm, 4);
    uLong comp_len = cstrm.total_out;

    // step 3: Operate - write compressed data to gz, read it back, then attempt inflate sync
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwr = 0;
    if (gw) gwr = gzwrite(gw, comp_buf, (unsigned int)(comp_len ? (unsigned int)comp_len : 0U));
    int gclosew = 0;
    if (gw) gclosew = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len ? comp_len : 1UL));
    memset(read_buf, 0, (int)(comp_len ? comp_len : 1UL));
    int grd = 0;
    if (gr) grd = gzread(gr, read_buf, (unsigned int)(comp_len ? (unsigned int)comp_len : 0U));
    int is_eof = 0;
    if (gr) is_eof = gzeof(gr);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(comp_len ? (uInt)comp_len : 0U);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int isync = inflateSync(&istrm);
    int iend = inflateEnd(&istrm);

    // step 4: Validate & Cleanup
    int dend = deflateEnd(&cstrm);
    int gcloser = 0;
    if (gr) gcloser = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)cinit;
    (void)dret;
    (void)comp_len;
    (void)gwr;
    (void)gclosew;
    (void)grd;
    (void)is_eof;
    (void)iinit;
    (void)isync;
    (void)iend;
    (void)dend;
    (void)gcloser;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}