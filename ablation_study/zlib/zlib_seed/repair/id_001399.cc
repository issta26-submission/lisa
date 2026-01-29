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
//<ID> 1399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compress -> deflatePrime -> write gz -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_src = crc32(0L, src, (uInt)src_len);

    // step 2: Configure (prepare buffers and compress)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress(comp_buf, &destLen, src, src_len);
    uLong comp_len = (uLong)destLen;

    // step 3: Operate (initialize deflate stream, prime bits, write compressed data to gz)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&dstrm, 3, 5);
    int def_end_ret = deflateEnd(&dstrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gz, 8192);
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong crc_comp = crc32(0L, comp_buf, (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    (void)crc_src;
    (void)crc_comp;
    (void)comp_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_end_ret;
    (void)gzbuf_ret;
    (void)gw;
    (void)gzclose_ret;
    (void)version;
    free(comp_buf);
    // API sequence test completed successfully
    return 66;
}