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
//<ID> 1768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize deflate, set dictionary, init inflate2, write gz, crc32 verify";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init2_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dict_buf[16];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    /* take first 16 bytes from src as dictionary content */
    memcpy(dict_buf, src, (size_t)((src_len < (uLong)sizeof(dict_buf)) ? src_len : (uLong)sizeof(dict_buf)));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int setdict_ret = deflateSetDictionary(&def_strm, dict_buf, dict_len);

    // step 3: Operate
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong crc_after = crc32(0UL, src, (uInt)src_len);
    uLong crc_combined = crc32(crc_before, src, (uInt)src_len);

    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init2_ret;
    (void)setdict_ret;
    (void)gw_ret;
    (void)gzclose_ret;
    (void)crc_after;
    (void)crc_combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}