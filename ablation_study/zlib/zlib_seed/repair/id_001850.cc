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
//<ID> 1850
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: prepare -> deflate -> getdict -> write gz -> checksum";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong adler_src = adler32(0UL, src, (uInt)src_len);
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

    // step 3: Operate (get dictionary and write compressed data to gz via gzdopen)
    Bytef dict_buf[256];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    gzFile gzf = gzdopen(1, "wb");
    int gz_written = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end = deflateEnd(&strm);
    const char *err_msg = zError(def_ret);
    uLong adler_comp = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? (uInt)comp_len : 0U));
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)getdict_ret;
    (void)dict_len;
    (void)gz_written;
    (void)gzclose_ret;
    (void)def_end;
    (void)err_msg;
    (void)adler_src;
    (void)adler_comp;
    // API sequence test completed successfully
    return 66;
}