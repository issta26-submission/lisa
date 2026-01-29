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
//<ID> 415
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
    Bytef dictionary[] = "zlib_api_sequence_dictionary";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int deflate_set_dict_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 2: Configure / Query
    Bytef retrieved_dict[64];
    memset(retrieved_dict, 0, sizeof(retrieved_dict));
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int deflate_get_dict_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);
    uLong bound_estimate = deflateBound(&strm, (uLong)128);
    const z_crc_t *crc_table = get_crc_table();

    // step 3: Operate (write compressed data to a .gz file using gz* APIs)
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gz write, gzbuffer and deflateBound test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzflush_ret = gzflush(gz, 0);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)dictionary;
    (void)dict_len;
    (void)deflate_set_dict_ret;
    (void)retrieved_dict;
    (void)retrieved_len;
    (void)deflate_get_dict_ret;
    (void)bound_estimate;
    (void)crc_table;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}