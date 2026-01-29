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
//<ID> 1885
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> inflateSync -> inspect gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *ver = zlibVersion();
    int dinit = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;

    // step 2: Configure compressor and compress
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound + 16));
    memset(comp_buf, 0, (int)(bound + 16));
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;

    // step 3: Write compressed data to .gz, reopen and inspect gzeof; then prepare inflater and call inflateSync
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gzw_close = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int eof_before = gzeof(gzr);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0 ? (uInt)comp_len : 0U);
    uLong out_size = src_len + 32;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (int)out_size);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_size;
    int inf_ret = inflate(&istrm, 4);
    int sync_ret = inflateSync(&istrm);
    int eof_after = gzeof(gzr);

    // step 4: Validate & Cleanup
    int iend = inflateEnd(&istrm);
    int dend = deflateEnd(&dstrm);
    int gzr_close = gzclose(gzr);
    free(comp_buf);
    free(out_buf);
    (void)dinit;
    (void)def_ret;
    (void)comp_len;
    (void)gw;
    (void)gzw_close;
    (void)eof_before;
    (void)iinit;
    (void)inf_ret;
    (void)sync_ret;
    (void)eof_after;
    (void)iend;
    (void)dend;
    (void)gzr_close;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}