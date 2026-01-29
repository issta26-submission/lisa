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
//<ID> 683
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
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong decompBound = INPUT_LEN * 2;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBound);
    memset(decompBuf, 0, (size_t)decompBound);

    // step 2: Initialize, configure and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, run inflate, call inflateSync and compute checksum
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = decompBuf;
    inf.avail_out = (uInt)decompBound;
    inflate(&inf, 0);
    inflateSync(&inf);
    uLong checksum = adler32_z(0UL, decompBuf, (z_size_t)inf.total_out);

    // step 4: Write decompressed data to a gz file, cleanup inflate and other resources
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decompBuf, (unsigned int)inf.total_out);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(decompBuf);
    (void)ver;
    (void)compBound;
    (void)compLen;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}