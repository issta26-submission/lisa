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
//<ID> 1859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzdopen/gzwrite -> get dictionary -> adler32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and buffers
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Write compressed data via gzdopen/gzwrite and extract dictionary & checksums
    gzFile gzf = gzdopen(1, "wb");
    int gz_written = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int gz_close_ret = gzclose(gzf);
    Bytef dict_buf[256];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    uLong adler_src = adler32(0UL, src, (uInt)src_len);
    uLong adler_comp = adler32(0UL, comp_buf, (uInt)(comp_len > 0 ? comp_len : 0UL));
    const char *err_str = zError(def_ret);

    // step 4: Cleanup & Validate
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_written;
    (void)gz_close_ret;
    (void)getdict_ret;
    (void)dict_len;
    (void)adler_src;
    (void)adler_comp;
    (void)err_str;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}