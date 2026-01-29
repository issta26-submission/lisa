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
//<ID> 1838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> get dictionary -> compute crc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Operate (write compressed data to gz file and query position)
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    off64_t pos64 = gztell64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    Bytef dict_buf[256];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int def_end = deflateEnd(&strm);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)gz_written;
    (void)pos64;
    (void)gzclose_ret;
    (void)getdict_ret;
    (void)dict_len;
    (void)crc_src;
    (void)crc_comp;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}