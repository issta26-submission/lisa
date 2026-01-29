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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress using the simple compress() API
    const Bytef source[] = "zlib API sequence payload demonstrating compress/deflate/inflateBack usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(source_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    uLongf comp1_len = (uLongf)bound1;
    int comp_ret = compress(comp1, &comp1_len, source, source_len);

    // step 2: Perform a deflate stream compression (deflateInit_/deflate) and finalize with deflateEnd
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound2 = deflateBound(&def_strm, source_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp2;
    def_strm.avail_out = (uInt)bound2;
    int def_ret = deflate(&def_strm, 4);
    uLong comp2_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize a regular inflate stream and use inflate to recover data; also initialize inflateBack and finalize it with inflateBackEnd
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp2;
    inf_strm.avail_in = (uInt)(comp2_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp2_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);
    z_stream infb_strm;
    memset(&infb_strm, 0, (int)sizeof(infb_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_init_ret = inflateBackInit_(&infb_strm, 15, window, version, (int)sizeof(z_stream));
    int infb_end_ret = inflateBackEnd(&infb_strm);

    // step 4: Validate checksums and clean up all resources
    uLong checksum1 = crc32(0UL, comp1, (uInt)(comp1_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp1_len));
    uLong checksum2 = crc32(0UL, out_buf, (uInt)(out_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_size));
    free(comp1);
    free(comp2);
    free(out_buf);
    free(window);

    (void)comp_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)infb_init_ret;
    (void)infb_end_ret;
    (void)comp1_len;
    (void)comp2_size;
    (void)checksum1;
    (void)checksum2;
    (void)version;
    (void)bound1;
    (void)bound2;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}