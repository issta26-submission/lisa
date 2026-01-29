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
//<ID> 848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "The quick brown fox jumps over the lazy dog";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong crc_initial = crc32(0L, sample, (uInt)sample_len);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);

    // step 2: Configure
    int inf_set_dict = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int unget_ret = gzungetc('A', gzr);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def, &pending, &bits);
    uLong crc_after = crc32(crc_initial, comp_buf, (uInt)comp_len);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl_r = gzclose(gzr);
    free(comp_buf);
    (void)version;
    (void)def_init;
    (void)out_bound;
    (void)inf_init;
    (void)inf_set_dict;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)unget_ret;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)crc_initial;
    (void)crc_after;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}