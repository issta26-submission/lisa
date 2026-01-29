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
//<ID> 858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Zlib API sequence sample data for compression and inflation";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzseek(gzf, 0, 0);
    off_t current_offset = gzoffset(gzf);

    // step 3: Operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int prime_ret = inflatePrime(&inf, 3, 5);
    Bytef dict_buf[64];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = inflateGetDictionary(&inf, dict_buf, &dict_len);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)pos_after_write;
    (void)current_offset;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl;
    // API sequence test completed successfully
    return 66;
}