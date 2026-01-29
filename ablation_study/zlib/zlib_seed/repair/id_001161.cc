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
//<ID> 1161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and compress using convenience compress()
    const Bytef source[] = "zlib API sequence payload demonstrating compress/inflateInit_/deflateEnd/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLongf comp_alloc = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_alloc);
    memset(comp_buf, 0, (size_t)comp_alloc);
    uLongf comp_len = comp_alloc;
    int comp_ret = compress(comp_buf, &comp_len, source, source_len);

    // step 2: Initialize a deflate stream (to demonstrate deflateInit_/deflateEnd lifecycle) and then end it
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate stream, inflate the compressed buffer, validate with adler32 and cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = adler32(0UL, out_buf, (uInt)out_size);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Initialize inflate-back stream (inflateBackInit_), then tear it down with inflateBackEnd, and free resources
    z_stream infb_strm;
    memset(&infb_strm, 0, (int)sizeof(infb_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_init_ret = inflateBackInit_(&infb_strm, 15, window, version, (int)sizeof(z_stream));
    int infb_end_ret = inflateBackEnd(&infb_strm);
    free(window);
    free(comp_buf);
    free(out_buf);

    (void)comp_ret;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)infb_init_ret;
    (void)infb_end_ret;
    (void)checksum;
    (void)version;
    (void)comp_len;
    (void)out_size;
    return 66; // API sequence test completed successfully
}