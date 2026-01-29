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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Zlib API sequence test payload";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&strm, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    strm.next_in = (Bytef *)sample;
    strm.avail_in = (uInt)sample_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret1 = deflate(&strm, 4);
    int reset_ret = deflateReset(&strm);
    strm.next_in = (Bytef *)sample;
    strm.avail_in = (uInt)sample_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)out_bound;
    int def_ret2 = deflate(&strm, 4);

    // step 3: Operate
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    uLong comp_len = strm.total_out;
    const char *fname = "test_zlib_api_sequence_temp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t gz_pos = gztell(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int ib_end = inflateBackEnd(&strm);
    int def_end = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret1;
    (void)reset_ret;
    (void)def_ret2;
    (void)ibinit_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_pos;
    (void)gz_cl;
    (void)ib_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}