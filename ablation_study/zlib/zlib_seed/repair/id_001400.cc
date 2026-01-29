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
//<ID> 1400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> write gz with gzputs -> inflate with inflatePrime -> record inflate result";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress(comp_buf, &destLen, src, src_len);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure (write original source to a gz file using gzputs)
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzputs(gzw, (const char *)src);
    int gzw_close = gzclose(gzw);

    // step 3: Operate (inflate the compressed buffer using inflateInit_, prime, inflate and call inflateBackEnd)
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
    int back_ret = inflateBackEnd(&istrm);

    // step 4: Validate & Cleanup (record inflate result string into the gz file, cleanup streams and buffers)
    const char *errstr = zError(inf_ret);
    gzFile gza = gzopen("test_zlib_api_sequence.gz", "ab");
    int ga = gzputs(gza, errstr);
    int gza_close = gzclose(gza);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp_buf);
    (void)src_len;
    (void)version;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw;
    (void)gzw_close;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)back_ret;
    (void)ga;
    (void)gza_close;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}