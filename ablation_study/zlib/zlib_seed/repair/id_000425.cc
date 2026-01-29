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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int deflate_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dictionary[] = "zlib_test_dictionary_for_sequence";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&def_strm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&inf_strm, dictionary, dict_len);

    // step 3: Operate
    Bytef src[] = "Example payload for zlib API sequence to be compressed then written to a gz file.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = deflateBound(&def_strm, src_len);
    uLongf dest_len = (uLongf)bound;
    Bytef *dest = (Bytef *)malloc((size_t)dest_len + 16);
    memset(dest, 0, (size_t)dest_len + 16);
    int comp_ret = compress(dest, &dest_len, src, src_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    unsigned int write_len = (unsigned int)(dest_len <= (uLongf)UINT_MAX ? (unsigned int)dest_len : (unsigned int)UINT_MAX);
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)dest, write_len);
    int gzflush_ret = gzflush(gz, 0);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&inf_strm);
    free(dest);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)bound;
    (void)comp_ret;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}