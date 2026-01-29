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
//<ID> 820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Zlib API sequence sample data for compression";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int reset_ret = deflateReset(&def);
    z_stream back;
    memset(&back, 0, sizeof(back));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&back, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos = gztell(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int ib_end = inflateBackEnd(&back);
    free(comp_buf);
    (void)init_ret;
    (void)reset_ret;
    (void)ibinit_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)pos;
    (void)gz_cl;
    (void)def_end;
    (void)ib_end;
    (void)version;
    (void)sample_len;
    // API sequence test completed successfully
    return 66;
}