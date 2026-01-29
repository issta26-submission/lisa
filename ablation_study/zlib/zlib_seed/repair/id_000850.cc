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
//<ID> 850
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample data for zlib usage and testing";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef preset_dict[] = "preset_dictionary_example";
    uInt preset_len = (uInt)(sizeof(preset_dict) - 1);
    int def_dict_ret = deflateSetDictionary(&def, preset_dict, preset_len);
    Bytef dictbuf[128];
    memset(dictbuf, 0, sizeof(dictbuf));
    uInt dictlen_out = 0;
    int inf_getdict_init = inflateGetDictionary(&inf, dictbuf, &dictlen_out);
    int inf_prime_init = inflatePrime(&inf, 0, 0);

    // step 3: Operate
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gzf);
    off_t seek_ret = gzseek(gzf, 0, 0);
    off_t pos_after_seek = gzoffset(gzf);

    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int inf_ret = inflate(&inf, 4);
    uInt dictlen_after = (uInt)sizeof(dictbuf);
    memset(dictbuf, 0, sizeof(dictbuf));
    int inf_getdict_after = inflateGetDictionary(&inf, dictbuf, &dictlen_after);
    int inf_prime_after = inflatePrime(&inf, 1, 1);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init;
    (void)inf_init;
    (void)preset_len;
    (void)def_dict_ret;
    (void)inf_getdict_init;
    (void)inf_prime_init;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)pos_after_write;
    (void)seek_ret;
    (void)pos_after_seek;
    (void)inf_ret;
    (void)inf_getdict_after;
    (void)inf_prime_after;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl;
    // API sequence test completed successfully
    return 66;
}