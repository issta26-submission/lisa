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
//<ID> 944
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload combining gz IO and inflate/getdictionary usage.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 8, 16, 32, 64);

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);

    // step 3: Operate
    int def_end = deflateEnd(&def_strm);
    gzFile gz_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gz_w, source, (unsigned int)src_len);
    int gzclose_w_ret = gzclose_w(gz_w);
    gzFile gz_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    Bytef *gz_read_buf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(gz_read_buf, 0, (size_t)(src_len + 128));
    z_size_t items_read = gzfread(gz_read_buf, 1, (z_size_t)(src_len + 128), gz_r);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 256));
    memset(decomp_buf, 0, (size_t)(src_len + 256));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 256);
    int inf_ret = inflate(&inf_strm, 4);
    Bytef dict_buf[64];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict = inflateGetDictionary(&inf_strm, dict_buf, &dict_len);
    int inf_end = inflateEnd(&inf_strm);
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(gz_read_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init;
    (void)tune_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_end;
    (void)gz_written;
    (void)gzclose_w_ret;
    (void)items_read;
    (void)inf_init;
    (void)inf_ret;
    (void)getdict;
    (void)inf_end;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}