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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for deflateBound, gzbuffer, deflateGetDictionary and get_crc_table test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char *ver = zlibVersion();
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_test_dictionary_sequence";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int dict_set_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 2: Configure
    Bytef dict_out[64];
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int dict_get_ret = deflateGetDictionary(&strm, dict_out, &dict_out_len);
    uLong comp_bound = deflateBound(&strm, (uLong)write_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t sample_crc = crc_tbl[0];

    // step 3: Operate
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, (unsigned int)comp_bound);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)filename;
    (void)deflate_init_ret;
    (void)dict_set_ret;
    (void)dict_get_ret;
    (void)dict_out_len;
    (void)sample_crc;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}