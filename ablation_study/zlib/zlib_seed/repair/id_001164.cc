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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it using compress()
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflateInit_/deflateEnd/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, source, source_len);

    // step 2: Initialize inflate stream and inflate compressed data
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 3: Initialize and immediately end a deflate stream to demonstrate deflate lifecycle
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Initialize inflate-back stream and then clean it up with inflateBackEnd(); cleanup buffers
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);
    free(window);
    free(comp_buf);
    free(out_buf);

    (void)comp_ret;
    (void)comp_len;
    (void)bound;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_size;
    (void)inf_end_ret;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)version;
    (void)source_len;
    return 66; // API sequence test completed successfully
}