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
//<ID> 732
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream dstr;
    memset(&dstr, 0, sizeof(dstr));
    int dinit_ret = deflateInit_(&dstr, 6, version, (int)sizeof(z_stream));
    dstr.next_in = plain;
    dstr.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&dstr, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    dstr.next_out = comp_buf;
    dstr.avail_out = (uInt)out_bound;

    // step 2: Configure
    z_stream istr;
    memset(&istr, 0, sizeof(istr));
    int iinit_ret = inflateInit_(&istr, version, (int)sizeof(z_stream));
    Bytef dict[256];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gw_written = gzwrite(gw, plain, (unsigned int)plain_len);
    int gw_closed = gzclose(gw);
    gzFile gr = gzopen(fname, "rb");

    // step 3: Operate
    int def_ret = deflate(&dstr, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&dstr, &pending, &bits);
    uLong total_out = dstr.total_out;
    int dend_ret = deflateEnd(&dstr);

    off64_t seek_res = gzseek64(gr, (off64_t)total_out, 0);
    int setdict_ret = inflateSetDictionary(&istr, dict, dict_len);

    // step 4: Validate / Cleanup
    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t part_len = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, part_len);

    int iend_ret = inflateEnd(&istr);
    int gr_closed = gzclose(gr);
    free(comp_buf);

    (void)dinit_ret;
    (void)out_bound;
    (void)dinit_ret;
    (void)dend_ret;
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)total_out;
    (void)seek_res;
    (void)setdict_ret;
    (void)gw_written;
    (void)gw_closed;
    (void)gr_closed;
    (void)iinit_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    (void)iend_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}