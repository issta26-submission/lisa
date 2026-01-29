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
//<ID> 852
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample data to compress for zlib API sequence including dictionary usage.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "sample_dictionary_v1";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_set_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    int inf_set_dict_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    const char *fname = "zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gzf);
    off_t seek_ret = gzseek(gzf, 0, 0);
    off_t pos_after_seek = gzoffset(gzf);

    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int inf_prime_ret = inflatePrime(&inf, 3, 1);
    int inf_ret = inflate(&inf, 4);

    Bytef *retrieved_dict = (Bytef *)malloc(64);
    memset(retrieved_dict, 0, 64);
    uInt retrieved_len = (uInt)64;
    int inf_get_dict_ret = inflateGetDictionary(&inf, retrieved_dict, &retrieved_len);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int gz_cl_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    free(retrieved_dict);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_set_dict_ret;
    (void)inf_set_dict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)pos_after_write;
    (void)seek_ret;
    (void)pos_after_seek;
    (void)inf_prime_ret;
    (void)inf_ret;
    (void)inf_get_dict_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_cl_ret;
    // API sequence test completed successfully
    return 66;
}