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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: compress with compress2, inflate normally, then exercise inflateBack init/prime/getdict/end.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)source_len + 64);
    memset(decomp_buf, 0, (size_t)source_len + 64);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(source_len + 64);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    uLong decomp_size = inf_strm.total_out;

    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&back_strm, 1, 1);
    uInt dict_len = (uInt)sizeof(window);
    Bytef dict_buf[32];
    memset(dict_buf, 0, sizeof(dict_buf));
    int getdict_ret = inflateGetDictionary(&back_strm, dict_buf, &dict_len);
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_size;
    (void)back_init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)back_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}