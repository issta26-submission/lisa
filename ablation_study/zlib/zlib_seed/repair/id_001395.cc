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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compute crc, one-shot compress, then stream-deflate prime and write via gz";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_orig = crc32(0L, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    defstrm.next_in = comp_buf;
    defstrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *def_out = (Bytef *)malloc((size_t)(bound + 64));
    memset(def_out, 0, (int)(bound + 64));
    defstrm.next_out = def_out;
    defstrm.avail_out = (uInt)((bound + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(bound + 64));
    int prime_ret = deflatePrime(&defstrm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong def_len = defstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gz, 16384);
    int gw = gzwrite(gz, def_out, (unsigned int)(def_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)def_len));
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    uLong crc_after = crc32(0L, def_out, (uInt)(def_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_len));
    (void)version;
    (void)def_init_ret;
    (void)comp_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)gw;
    (void)buf_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)crc_orig;
    (void)crc_after;
    free(comp_buf);
    free(def_out);
    // API sequence test completed successfully
    return 66;
}