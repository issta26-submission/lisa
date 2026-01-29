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
//<ID> 495
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
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize / Configure streams and open gzip file
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzflush(gz, 2);
    gzclose(gz);

    // step 3: Operate & Validate (decompress into outBuf and verify checksum)
    uLongf outLen = (uLongf)inputLen;
    uLong compLenU = (uLong)compLen;
    uncompress(outBuf, &outLen, compBuf, compLenU);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    (void)checksum;

    // step 4: Cleanup
    deflateEnd(&defStrm);
    inflateBackEnd(&backStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    free(window);

    return 66; // API sequence test completed successfully
}