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
//<ID> 607
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong bound = compressBound(INPUT_LEN);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input and compute CRCs
    compress(compBuf, &compLen, input, INPUT_LEN);
    uLong crc_input = crc32(0L, input, (uInt)INPUT_LEN);
    uLong crc_comp = crc32(0L, compBuf, (uInt)compLen);
    uLong crc_combined = crc32_combine(crc_input, crc_comp, (off_t)compLen);

    // step 3: Initialize inflate stream and prepare for decompression; write compressed data to a gz file
    memset(&infStrm, 0, sizeof(infStrm));
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(outBuf, 0, (size_t)INPUT_LEN);
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)INPUT_LEN;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose_w(gz);

    // step 4: Validate & cleanup
    (void)crc_combined;
    inflateEnd(&infStrm);
    free(input);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}