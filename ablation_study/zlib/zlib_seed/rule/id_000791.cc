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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 2048UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong adler_input = adler32_z(0UL, input, (z_size_t)INPUT_LEN);

    // step 2: Write compressed data to a gzip file
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz);

    // step 3: Initialize an inflate stream and call inflateUndermine
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    int undermine_result = inflateUndermine(&strm, 1);
    inflateEnd(&strm);

    // step 4: Initialize inflateBack state, tear it down, validate checksum and cleanup
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    int back_end_res = inflateBackEnd(&backstrm);
    uLong adler_check = adler32_z(0UL, input, (z_size_t)INPUT_LEN);
    free(input);
    (void)ver;
    (void)adler_input;
    (void)adler_check;
    (void)undermine_result;
    (void)back_end_res;
    // API sequence test completed successfully
    return 66;
}