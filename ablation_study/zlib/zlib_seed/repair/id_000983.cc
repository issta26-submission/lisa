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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: create compressed data with deflate, init inflateBack, prime and query dictionary, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure / Create compressed data
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate with inflateBackInit_, inflatePrime, inflateGetDictionary, inflateBackEnd
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned int window_size = 1u << 15; // typical 32KB window
    unsigned char *window = (unsigned char *)malloc((size_t)window_size);
    memset(window, 0, (size_t)window_size);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    back_strm.next_in = comp_buf;
    back_strm.avail_in = (uInt)comp_size;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(source_len + 64));
    memset(decomp_buf, 0, (size_t)(source_len + 64));
    back_strm.next_out = decomp_buf;
    back_strm.avail_out = (uInt)(source_len + 64);
    int prime_ret = inflatePrime(&back_strm, 3, 5);
    uInt dict_buf_size = 64;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_buf_size);
    memset(dict_buf, 0, (size_t)dict_buf_size);
    uInt dict_len = dict_buf_size;
    int getdict_ret = inflateGetDictionary(&back_strm, dict_buf, &dict_len);
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(window);
    free(decomp_buf);
    free(dict_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)prime_ret;
    (void)getdict_ret;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}