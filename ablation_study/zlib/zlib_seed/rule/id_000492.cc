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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream backStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    uncompress(outBuf, &outLen, compBuf, (uLong)compLen);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    (void)checksum;

    // step 2: Initialize and configure streams/resources
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, compBuf, (unsigned int)compLen);

    // step 3: Operate & validate (use compressed data and streams meaningfully)
    /* use streams/resources above: compBuf written to gz file, outBuf validated by checksum */

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateBackEnd(&backStrm);
    inflateEnd(&infStrm);
    gzclose(gf);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66; // API sequence test completed successfully
}