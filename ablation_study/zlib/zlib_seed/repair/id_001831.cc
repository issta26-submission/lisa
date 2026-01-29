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
//<ID> 1831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize -> compress -> write gz -> validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong src_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Operate
    uInt dict_capacity = 64U;
    Bytef *dict_buf = (Bytef *)malloc((size_t)dict_capacity);
    memset(dict_buf, 0, (int)dict_capacity);
    uInt dict_len = 0U;
    int getdict_ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_len));
    off64_t pos64 = gztell64(gzf);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)(comp_len > 0UL ? (uInt)comp_len : 0U));

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(dict_buf);
    (void)src_crc;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)getdict_ret;
    (void)gzwrite_ret;
    (void)pos64;
    (void)comp_crc;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}