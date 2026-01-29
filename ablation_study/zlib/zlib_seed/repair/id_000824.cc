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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample data for zlib API sequence test.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // Step 2: Configure
    int def_ret1 = deflate(&def, 4);
    uLong comp_len1 = def.total_out;
    int def_reset_ret = deflateReset(&def);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret2 = deflate(&def, 4);
    uLong comp_len2 = def.total_out;
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&inf, 15, window, version, (int)sizeof(z_stream));

    // Step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len2);
    off_t pos = gztell(gzf);
    int gz_cl = gzclose(gzf);

    // Step 4: Validate / Cleanup
    int ibend_ret = inflateBackEnd(&inf);
    int def_end = deflateEnd(&def);
    free(comp_buf);
    (void)init_ret;
    (void)out_bound;
    (void)def_ret1;
    (void)comp_len1;
    (void)def_reset_ret;
    (void)def_ret2;
    (void)comp_len2;
    (void)ibinit_ret;
    (void)fname;
    (void)gz_wr;
    (void)pos;
    (void)gz_cl;
    (void)ibend_ret;
    (void)def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}