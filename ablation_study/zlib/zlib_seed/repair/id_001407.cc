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
//<ID> 1407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> write gz -> inflatePrime -> inflate -> inflateBackEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gp = gzputs(gz, "zlib-metadata");
    int gzbuf_ret = gzbuffer(gz, 8192);

    // step 3: Operate
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int prime_ret = inflatePrime(&istrm, 3, 5);
    int inf_ret = inflate(&istrm, 0);
    const char *errstr = zError(inf_ret);

    // step 4: Validate & Cleanup
    int back_end_ret = inflateBackEnd(&istrm);
    int inf_end_ret = inflateEnd(&istrm);
    int gzclose_ret = gzclose(gz);
    (void)version;
    (void)comp_ret;
    (void)gp;
    (void)gzbuf_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)errstr;
    (void)back_end_ret;
    (void)inf_end_ret;
    (void)gzclose_ret;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}