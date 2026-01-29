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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compute crc -> prime deflate -> compress -> write gz with buffer";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong src_crc = crc32(0UL, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int def_prime_ret = deflatePrime(&defstrm, 3, 5);

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 3: Operate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gz, 16384);
    unsigned int write_len = (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : (unsigned int)comp_len);
    int gw = gzwrite(gz, comp_buf, write_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);
    int def_end_ret = deflateEnd(&defstrm);
    (void)version;
    (void)def_init_ret;
    (void)def_prime_ret;
    (void)bound;
    (void)comp_ret;
    (void)gzbuf_ret;
    (void)gw;
    (void)gzclose_ret;
    (void)src_crc;
    (void)comp_crc;
    (void)def_end_ret;
    free(comp_buf);
    // API sequence test completed successfully
    return 66;
}