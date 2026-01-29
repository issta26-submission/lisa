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
//<ID> 600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char * ver = zlibVersion();
    const uLong INPUT_LEN = (uLong)128;
    Bytef * input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLongf compLen = (uLongf)compressBound(INPUT_LEN);
    Bytef * compBuf = (Bytef *)malloc((size_t)compLen);
    memset(compBuf, 0, (size_t)compLen);

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, INPUT_LEN);

    // step 3: Initialize inflate stream and compute CRCs for meaningful data flow
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = input;
    infStrm.avail_out = (uInt)INPUT_LEN;
    const uLong HALF = INPUT_LEN / 2;
    uLong crc1 = crc32(0L, input, (uInt)HALF);
    uLong crc2 = crc32(0L, input + HALF, (uInt)HALF);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)HALF);
    infStrm.adler = combined_crc;

    // step 4: Write compressed data to a gz file and cleanup
    gzFile gz = gzopen64("tmp_test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose_w(gz);
    inflateEnd(&infStrm);
    free(compBuf);
    free(input);

    // API sequence test completed successfully
    return 66;
}