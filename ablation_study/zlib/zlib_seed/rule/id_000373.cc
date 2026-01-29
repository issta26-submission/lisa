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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    z_stream strm2;
    memset(&strm, 0, sizeof(strm));
    memset(&strm2, 0, sizeof(strm2));
    unsigned char input[256];
    memset(input, 'Q', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong origin_adler = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;

    // step 2: Compress the input to produce compressed data used by inflateBack/inflate
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 3: Initialize inflateBack and inflate, then perform decompression operations
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compDestLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)inputLen;
    inflateBack(&strm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&strm);
    inflateInit_(&strm2, zlibVersion(), (int)sizeof(z_stream));
    strm2.next_in = compBuf;
    strm2.avail_in = (uInt)compDestLen;
    strm2.next_out = outBuf;
    strm2.avail_out = (uInt)inputLen;
    inflate(&strm2, 0);
    uLong decompressed_total = (uLong)strm2.total_out;
    uLong out_adler = adler32(0L, (const Bytef *)outBuf, (uInt)decompressed_total);

    // step 4: Write compressed and decompressed data to a gz file and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzwrite(gz, outBuf, (unsigned int)decompressed_total);
    gzclose(gz);
    inflateEnd(&strm2);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}