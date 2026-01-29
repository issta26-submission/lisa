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
//<ID> 1801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> extract dict -> write gz -> inflateBack init/end";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 2: Retrieve dictionary from deflate internal state and finish deflate
    uInt dict_capacity = 32768U;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_capacity);
    memset(dict_buf, 0, (int)dict_capacity);
    uInt dict_len = dict_capacity;
    int def_getdict_ret = deflateGetDictionary(&def_strm, dict_buf, &dict_len);
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed output to a gz file and close it
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gzclose_ret = gzclose(gzf);

    // step 4: Initialize inflateBack with a sliding window, then end inflateBack and cleanup
    unsigned char *window_buf = (unsigned char *)malloc((size_t)dict_capacity);
    memset(window_buf, 0, (int)dict_capacity);
    int inf_back_init_ret = inflateBackInit_(&inf_strm, 15, window_buf, ver, (int)sizeof(z_stream));
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    free(comp_buf);
    free(dict_buf);
    free(window_buf);
    (void)src_len;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_getdict_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_back_init_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}