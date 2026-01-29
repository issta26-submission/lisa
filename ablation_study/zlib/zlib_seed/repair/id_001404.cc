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
//<ID> 1404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib sequence: example message for gzputs and inflatePrime";
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
    int gw = gzputs(gz, (const char *)src);
    int gzclose_ret = gzclose(gz);

    // step 3: Operate
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp, 0, (int)(src_len + 16));
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(src_len + 16);
    int prime_ret = inflatePrime(&istrm, 3, 5);
    int back_ret = inflateBackEnd(&istrm);
    const char *errstr = zError(back_ret);

    // step 4: Validate & Cleanup
    uLong crc_src = crc32(0L, src, (uInt)src_len);
    (void)version;
    (void)comp_ret;
    (void)gw;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)errstr;
    (void)crc_src;
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}