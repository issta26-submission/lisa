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
//<ID> 1000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: create compressed data, copy inflater, set dictionary, and exercise gz read.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    (void)def_ret;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_end_ret;

    // step 2: Configure
    z_stream inf_strm;
    z_stream copy_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    (void)inf_init_ret;
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    int copy_ret = inflateCopy(&copy_strm, &inf_strm);
    (void)copy_ret;
    uInt dictLen = (uInt)4;
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, source, (size_t)dictLen);
    int setdict_ret = inflateSetDictionary(&copy_strm, dict, dictLen);
    (void)setdict_ret;

    // step 3: Operate
    int inf_ret = inflate(&copy_strm, 0);
    (void)inf_ret;
    int inf_end_copy = inflateEnd(&copy_strm);
    int inf_end_orig = inflateEnd(&inf_strm);
    (void)inf_end_copy;
    (void)inf_end_orig;
    gzFile gzout = gzopen("zlib_temp.gz", "wb");
    int gz_write_ret = gzwrite(gzout, comp_buf, (unsigned int)comp_size);
    (void)gz_write_ret;
    int gzclose_w_ret = gzclose(gzout);
    (void)gzclose_w_ret;
    gzFile gzin = gzopen("zlib_temp.gz", "rb");
    int first_char = gzgetc(gzin);
    (void)first_char;
    int gzclose_r_ret = gzclose(gzin);
    (void)gzclose_r_ret;

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(dict);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
    // API sequence test completed successfully
}