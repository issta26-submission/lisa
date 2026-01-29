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
//<ID> 1408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib sequence sample data for inflatePrime -> inflateBackEnd -> gzputs -> zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gp1 = gzputs(gz, "Zlib metadata: original_len= ");
    (void)gp1;
    (void)comp_ret;

    // step 2: Configure
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 64);
    (void)inf_init_ret;

    // step 3: Operate
    int prime_ret = inflatePrime(&istrm, 3, 5);
    int back_end_ret = inflateBackEnd(&istrm);
    const char *err_str = zError(back_end_ret);
    int gp2 = gzputs(gz, err_str);
    (void)prime_ret;
    (void)gp2;

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&istrm);
    int gzclose_ret = gzclose(gz);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)inf_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}