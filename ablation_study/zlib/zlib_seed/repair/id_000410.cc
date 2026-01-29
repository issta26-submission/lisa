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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef orig_dict[] = "zlib_test_dictionary_for_get_dictionary";
    uInt orig_dict_len = (uInt)(sizeof(orig_dict) - 1);
    int deflate_set_dict_ret = deflateSetDictionary(&strm, orig_dict, orig_dict_len);

    // step 2: Configure
    Bytef retrieved_dict[128];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int deflate_get_dict_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    uLong sourceLen = (uLong)retrieved_len;
    uLong bound = deflateBound(&strm, sourceLen);
    Bytef dest_buf[512];
    (void)dest_buf; // dest_buf reserved according to bound (no dynamic alloc)
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)retrieved_dict, retrieved_len);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc_entry = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    uLong used_crc = (uLong)first_crc_entry;
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_set_dict_ret;
    (void)deflate_get_dict_ret;
    (void)sourceLen;
    (void)bound;
    (void)filename;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)used_crc;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}