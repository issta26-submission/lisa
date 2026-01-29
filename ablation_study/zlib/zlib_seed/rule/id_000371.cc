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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream backStrm;
    z_stream infStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum_input = adler32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Compress the input
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6U);

    // step 3: Decompression using inflateBack and inflate
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf1 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf1, 0, (size_t)inputLen);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf1;
    backStrm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backStrm);

    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compDestLen;
    infStrm.next_out = outBuf2;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    uLong checksum_out = adler32(0L, outBuf2, (uInt)inputLen);
    inflateEnd(&infStrm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzclose(gz);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    (void)checksum_input;
    (void)checksum_out;
    return 66; // API sequence test completed successfully
}