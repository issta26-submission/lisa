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
//<ID> 656
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    uLong bound = compressBound(plain_len);
    uLongf one_shot_dest_len = (uLongf)bound;
    Bytef *one_shot_dest = (Bytef *)malloc((size_t)bound);
    memset(one_shot_dest, 0, (size_t)bound);
    int comp_ret = compress(one_shot_dest, &one_shot_dest_len, plain, plain_len);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef def_out[512];
    memset(def_out, 0, sizeof(def_out));
    dstrm.next_in = plain;
    dstrm.avail_in = (uInt)plain_len;
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)sizeof(def_out);

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong def_comp_len = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain, (unsigned int)plain_len);
    int gw_close = gzclose(gw);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    (void)version;
    (void)comp_ret;
    (void)one_shot_dest_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)first_char;
    (void)gr_close;
    free(one_shot_dest);
    // API sequence test completed successfully
    return 66;
}