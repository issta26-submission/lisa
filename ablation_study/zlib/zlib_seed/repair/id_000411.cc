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
//<ID> 411
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
    Bytef dictionary[] = "zlib_test_dict_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int setdict_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 2: Configure
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite and deflateBound usage.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    uLong max_bound = deflateBound(&strm, (uLong)write_len);
    Bytef getbuf[128];
    uInt getbuf_len = (uInt)sizeof(getbuf);
    int getdict_ret = deflateGetDictionary(&strm, getbuf, &getbuf_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)setdict_ret;
    (void)max_bound;
    (void)getdict_ret;
    (void)getbuf_len;
    (void)first_crc;
    (void)filename;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}