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
//<ID> 1390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compute crc, deflate with priming, compress, write with gz and buffer";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0L, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *def_comp = (Bytef *)malloc((size_t)bound);
    memset(def_comp, 0, (int)bound);
    defstrm.next_out = def_comp;
    defstrm.avail_out = (uInt)bound;
    int prime_ret = deflatePrime(&defstrm, 1, 1);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong def_len = defstrm.total_out;
    uLongf comp2_len = compressBound(src_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)comp2_len);
    memset(comp2, 0, (int)comp2_len);
    uLongf comp2_len_actual = comp2_len;
    int comp_ret = compress(comp2, &comp2_len_actual, src, src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gz, 8192U);
    int gw = gzwrite(gz, comp2, (unsigned int)comp2_len_actual);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    uLong crc_after = crc32(crc_initial, comp2, (uInt)comp2_len_actual);
    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_len;
    (void)comp_ret;
    (void)buf_ret;
    (void)gw;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)crc_after;
    free(def_comp);
    free(comp2);
    // API sequence test completed successfully
    return 66;
}