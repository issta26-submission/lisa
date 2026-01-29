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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_test_dictionary_for_get";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];

    // step 2: Configure
    int deflate_set_dict_ret = deflateSetDictionary(&strm, dictionary, dict_len);
    Bytef dict_get_buffer[64];
    uInt dict_get_len = (uInt)sizeof(dict_get_buffer);
    int deflate_get_dict_ret = deflateGetDictionary(&strm, dict_get_buffer, &dict_get_len);

    // step 3: Operate
    Bytef src_data[] = "Payload to estimate deflateBound and write via gz";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uLong bound = deflateBound(&strm, src_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 4096U);
    unsigned int write_len = (unsigned int)src_len;
    int gzwrite_ret = gzwrite(gz, src_data, write_len);
    int gzflush_ret = gzflush(gz, 0);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)dictionary;
    (void)dict_len;
    (void)crc_tbl;
    (void)first_crc;
    (void)deflate_set_dict_ret;
    (void)dict_get_buffer;
    (void)dict_get_len;
    (void)deflate_get_dict_ret;
    (void)bound;
    (void)filename;
    (void)gzbuffer_ret;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}