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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and prepare deflate stream
    const Bytef source[] = "zlib API sequence payload demonstrating deflateInit_/deflate/compress/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Also compress the same source with the convenience API and validate sizes
    uLongf comp2_len = (uLongf)compressBound(source_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_len);
    memset(comp2_buf, 0, (int)comp2_len);
    int comp_ret = compress(comp2_buf, &comp2_len, source, source_len);

    // step 3: Initialize inflate stream and perform inflate-back initialization/cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_end_ret = inflateEnd(&inf_strm);
    z_stream infb_strm;
    memset(&infb_strm, 0, (int)sizeof(infb_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_init_ret = inflateBackInit_(&infb_strm, 15, window, version, (int)sizeof(z_stream));
    int infb_end_ret = inflateBackEnd(&infb_strm);
    free(window);

    // step 4: Cleanup resources and finalize
    free(comp_buf);
    free(comp2_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_ret;
    (void)comp_size;
    (void)comp2_len;
    (void)inf_init_ret;
    (void)inf_end_ret;
    (void)infb_init_ret;
    (void)infb_end_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66; // API sequence test completed successfully
}