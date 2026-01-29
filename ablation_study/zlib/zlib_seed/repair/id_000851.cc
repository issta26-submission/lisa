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
//<ID> 851
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "The quick brown fox jumps over the lazy dog - zlib test sample";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "zlib_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence.tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t seek_ret = gzseek(gzf, 0, 0);
    off_t pos_after_seek = gzoffset(gzf);

    // step 4: Validate / Cleanup
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int dict_get_ret = inflateGetDictionary(&inf, dict_out, &dict_out_len);
    int prime_ret = inflatePrime(&inf, 0, 0);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init;
    (void)inf_init;
    (void)def_dict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)seek_ret;
    (void)pos_after_seek;
    (void)dict_get_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl;
    // API sequence test completed successfully
    return 66;
}