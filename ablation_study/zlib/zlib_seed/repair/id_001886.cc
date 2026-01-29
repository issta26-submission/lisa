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
//<ID> 1886
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite -> gzread -> inflate -> inflateSync";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *ver = zlibVersion();
    int dinit = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 2: Compress source data
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;

    // step 3: Write compressed data to a gzip file and read it back
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_wr = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len > 0 ? (size_t)comp_len : 1));
    memset(read_buf, 0, (int)(comp_len > 0 ? (int)comp_len : 1));
    int gz_rd = gzread(gzr, read_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int eof_flag = gzeof(gzr);

    // step 4: Inflate read data, attempt sync, and cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(gz_rd > 0 ? (uInt)gz_rd : 0U);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 4);
    int sync_ret = inflateSync(&istrm);
    int iend = inflateEnd(&istrm);
    int dend = deflateEnd(&dstrm);
    int gz_close_r = gzclose(gzr);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)dinit;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)gz_rd;
    (void)eof_flag;
    (void)iinit;
    (void)inf_ret;
    (void)sync_ret;
    (void)iend;
    (void)dend;
    (void)gz_close_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}