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
//<ID> 1836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize, set/get dictionary, deflate, write gz, validate CRC";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef preset_dict[] = "example_dictionary_for_deflate";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);
    Bytef *retrieved_dict = (Bytef *)malloc((size_t)preset_dict_len);
    memset(retrieved_dict, 0, (int)preset_dict_len);
    uInt retrieved_len = preset_dict_len;
    int get_dict_ret = deflateGetDictionary(&def_strm, retrieved_dict, &retrieved_len);

    // step 2: Configure & Compress
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Operate
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t wrote = gzfwrite(comp_buf, 1, (z_size_t)(comp_len > 0 ? comp_len : 0UL), gzf);
    off64_t pos64 = gztell64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)(comp_len > 0 ? (uInt)comp_len : 0U));
    int end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(retrieved_dict);
    (void)ver;
    (void)init_ret;
    (void)set_dict_ret;
    (void)get_dict_ret;
    (void)retrieved_len;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)wrote;
    (void)pos64;
    (void)gzclose_ret;
    (void)comp_crc;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}