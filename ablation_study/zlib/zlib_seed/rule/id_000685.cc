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
//<ID> 685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong checksum_before = adler32_z(0L, input, (z_size_t)src_len);

    // step 2: Compress using deflate APIs
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong comp_len = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, perform inflate, call inflateSync and inflateEnd
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)comp_len;
    Bytef *outBuf = (Bytef *)malloc((size_t)src_len);
    memset(outBuf, 0, (size_t)src_len);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)src_len;
    inflate(&inf, 0);
    inflateSync(&inf);
    uLong checksum_after = adler32_z(0L, outBuf, (z_size_t)src_len);
    inflateEnd(&inf);

    // step 4: Write decompressed data to a gz file, cleanup resources
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)src_len);
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)checksum_before;
    (void)checksum_after;
    // API sequence test completed successfully
    return 66;
}