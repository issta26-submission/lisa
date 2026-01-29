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
//<ID> 1401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: compress -> write gz header -> inflate with prime -> cleanup";
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
    int gp = gzputs(gz, "zlib-metadata: sequence-run\n");
    (void)gp;
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    (void)gw;

    // step 3: Operate
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 64);
    int prime_ret = inflatePrime(&istrm, 3, 5);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    const char *errstr = zError(prime_ret);
    (void)errstr;
    int ib_end = inflateBackEnd(&istrm);
    (void)ib_end;
    int inf_end = inflateEnd(&istrm);
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end;
    int gzclose_ret = gzclose(gz);
    (void)gzclose_ret;
    (void)comp_ret;
    free(comp_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}