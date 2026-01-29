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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib: quick test payload written via gzopen64 and read back with gzgetc_/gzfread.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef read_buf[512];
    z_size_t items_read;
    int first_char;
    const char * ver;
    gzFile gz_write = (gzFile)0;
    gzFile gz_read = (gzFile)0;
    int ret_inf_init;
    int ret_setdict;
    int ret_inf_end;
    int ret_gz_close_write;
    int ret_gz_close_read;
    int ret_gz_write;
    int ret_gz_flush;
    Bytef dictionary[] = "zlib_dict_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations
    gz_write = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz_write, input_buf, (unsigned int)input_len);
    ret_gz_flush = gzflush(gz_write, 1);
    ret_gz_close_write = gzclose(gz_write);
    gz_read = gzopen64("test_zlib_api_sequence.gz", "rb");
    first_char = gzgetc_(gz_read);
    items_read = gzfread(read_buf, 1, (z_size_t)sizeof(read_buf), gz_read);
    ret_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)dictionary, dict_len);

    // step 4: Cleanup
    ret_inf_end = inflateEnd(&inf_strm);
    ret_gz_close_read = gzclose(gz_read);
    (void)ret_inf_init;
    (void)ret_setdict;
    (void)ret_inf_end;
    (void)ret_gz_close_write;
    (void)ret_gz_close_read;
    (void)ret_gz_write;
    (void)ret_gz_flush;
    (void)first_char;
    (void)items_read;
    (void)ver;
    (void)input_len;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}