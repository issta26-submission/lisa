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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    const unsigned int INPUT_LEN = 256;
    Bytef * input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong inputLen = (uLong)INPUT_LEN;
    uLong bound = compressBound(inputLen);
    uLongf compLen = (uLongf)bound;
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Compress data
    compress(compBuf, &compLen, input, inputLen);

    // step 3: Initialize inflate and decompress
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    inflateEnd(&infStrm);

    // step 4: Compute CRC combine, write compressed data to gz, cleanup
    uLong crc1 = crc32(0L, input, (uInt)inputLen);
    uLong crc2 = crc32(0L, outBuf, (uInt)inputLen);
    uLong combined = crc32_combine(crc1, crc2, (off_t)inputLen);
    (void)combined;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose_w(gz);
    free(input);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}