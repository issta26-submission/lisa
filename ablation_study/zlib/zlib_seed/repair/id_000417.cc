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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const z_crc_t *crc_tbl = get_crc_table();
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_test_dictionary_data";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int deflate_set_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 2: Configure
    Bytef retrieved_dict[64];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int deflate_get_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);
    uLong bound = deflateBound(&strm, (uLong)1024);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite, gzbuffer and deflateBound test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    uLong crc_val = crc32(0UL, write_buf, (uInt)write_len);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzflush_ret = gzflush(gz, 0);
    off_t offset_after_write = gzoffset(gz);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)crc_tbl;
    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_set_ret;
    (void)deflate_get_ret;
    (void)bound;
    (void)filename;
    (void)write_len;
    (void)crc_val;
    (void)gz;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)offset_after_write;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}