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
//<ID> 412
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
    Bytef dictionary[] = "zlib_dictionary_example";
    uInt dictionary_len = (uInt)(sizeof(dictionary) - 1);

    // step 2: Configure
    int def_set_ret = deflateSetDictionary(&strm, dictionary, dictionary_len);
    Bytef got_dict[64];
    uInt got_len = (uInt)sizeof(got_dict);
    int def_get_ret = deflateGetDictionary(&strm, got_dict, &got_len);
    uLong bound = deflateBound(&strm, (uLong)got_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    Bytef *comp_buf = new Bytef[(size_t)bound + 1];

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)got_dict, (unsigned int)got_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    delete [] comp_buf;
    (void)ver;
    (void)deflate_init_ret;
    (void)dictionary_len;
    (void)def_set_ret;
    (void)def_get_ret;
    (void)bound;
    (void)first_crc;
    (void)filename;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}