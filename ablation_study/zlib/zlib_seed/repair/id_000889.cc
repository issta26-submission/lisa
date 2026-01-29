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
//<ID> 889
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and checksum.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong src_adler = adler32(0UL, source, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_orig = gzwrite(gzf, source, (unsigned int)src_len);
    int gz_wr_comp = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)src_adler;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr_orig;
    (void)gz_wr_comp;
    (void)gz_cl;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}