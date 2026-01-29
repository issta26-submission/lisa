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
//<ID> 418
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
    Bytef dict_set[] = "example_zlib_dictionary";
    uInt dict_len_set = (uInt)(sizeof(dict_set) - 1);
    int deflate_set_ret = deflateSetDictionary(&strm, dict_set, dict_len_set);
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_len_out = (uInt)sizeof(dict_out);
    int deflate_get_ret = deflateGetDictionary(&strm, dict_out, &dict_len_out);

    // step 2: Configure / compute bounds and access CRC table
    uLong bound = deflateBound(&strm, (uLong)1234);
    const z_crc_t *crc_tab = get_crc_table();
    z_crc_t first_crc = crc_tab[0];

    // step 3: Operate - write data to a gz file using computed parameters
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[512];
    memset(write_buf, (int)(first_crc & 0xFF), sizeof(write_buf));
    unsigned int write_len = (unsigned int)((bound & 511u) + 1u);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, write_buf, write_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)dict_set;
    (void)dict_len_set;
    (void)deflate_set_ret;
    (void)dict_out;
    (void)dict_len_out;
    (void)deflate_get_ret;
    (void)bound;
    (void)crc_tab;
    (void)first_crc;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}