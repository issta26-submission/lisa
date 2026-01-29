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
//<ID> 606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong IN_LEN = (uLong)256;
    Bytef * inBuf = (Bytef *)malloc((size_t)IN_LEN);
    memset(inBuf, 'X', (size_t)IN_LEN);
    uLong inLen = IN_LEN;
    uLong compBound = compressBound(inLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;

    // step 2: Compress input
    compress(compBuf, &compLen, inBuf, inLen);

    // step 3: Initialize inflate stream and write compressed data to a gzip file
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    const char * ver = zlibVersion();
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inLen);
    memset(outBuf, 0, (size_t)inLen);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)inLen;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose_w(gz);

    // step 4: Compute checksums, cleanup
    uLong crc1 = crc32(0L, inBuf, (uInt)inLen);
    uLong crc2 = crc32(0L, compBuf, (uInt)compLen);
    uLong combined = crc32_combine(crc1, crc2, (off_t)compLen);
    (void)combined;
    inflateEnd(&inf);
    free(inBuf);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}