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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream infStrm;
    z_stream backStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize & Configure
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLong compLenU = (uLong)((uLong)bound - (uLong)defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Operate & Validate
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, input, (unsigned int)inputLen);
    gzclose(gz);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    uLongf outLen = (uLongf)inputLen;
    uncompress(outBuf, &outLen, compBuf, compLenU);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)outLen);
    (void)checksum;

    // step 4: Cleanup
    inflateEnd(&infStrm);
    inflateBackEnd(&backStrm);
    free(compBuf);
    free(outBuf);
    free(window);

    return 66; // API sequence test completed successfully
}