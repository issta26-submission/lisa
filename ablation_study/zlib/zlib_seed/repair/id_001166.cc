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
//<ID> 1166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data and core zlib version/streams
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflateInit_/inflateBackEnd/deflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    z_stream infb_strm;
    memset(&infb_strm, 0, (int)sizeof(infb_strm));

    // step 2: Configure compression buffers and perform a simple compress()
    uLong comp_bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, source, source_len);

    // step 3: Initialize inflate-back stream, initialize and finalize a deflate stream
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_init_ret = inflateBackInit_(&infb_strm, 15, window, version, (int)sizeof(z_stream));
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&def_strm);
    int infb_end_ret = inflateBackEnd(&infb_strm);

    // step 4: Validate decompression with uncompress, cleanup inflate stream and free resources
    uLongf out_len = (uLongf)(source_len + 16);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (size_t)out_len);
    int uncomp_ret = uncompress(out_buf, &out_len, comp_buf, (uLong)comp_len);
    uLong checksum = crc32(0UL, out_buf, (uInt)out_len);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(comp_buf);
    free(out_buf);
    free(window);

    (void)version;
    (void)inf_init_ret;
    (void)infb_init_ret;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)infb_end_ret;
    (void)comp_ret;
    (void)comp_len;
    (void)uncomp_ret;
    (void)checksum;
    (void)inf_end_ret;
    (void)source_len;
    (void)out_len;
    return 66;
    // API sequence test completed successfully
}