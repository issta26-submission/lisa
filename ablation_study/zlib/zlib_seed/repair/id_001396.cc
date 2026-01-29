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
//<ID> 1396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compute crc32, compress, deflatePrime, write via gz with buffering";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_src = crc32(0L, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    uLong comp2_bound = compressBound(src_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)comp2_bound);
    memset(comp2, 0, (int)comp2_bound);
    uLongf comp2_len = (uLongf)comp2_bound;

    // step 3: Operate
    int prime_ret = deflatePrime(&defstrm, 3, 5);
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int comp_ret = compress(comp2, &comp2_len, src, src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gb = gzbuffer(gz, 8192);
    int gw = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gc = gzclose(gz);
    uLong crc_comp = crc32(0L, comp_buf, (uInt)comp_len);
    uLong crc_comp2 = crc32(0L, comp2, (uInt)comp2_len);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    free(comp2);
    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)comp_ret;
    (void)gb;
    (void)gw;
    (void)gc;
    (void)crc_src;
    (void)crc_comp;
    (void)crc_comp2;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}