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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    z_stream defStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char orig[256];
    memset(orig, 'Z', sizeof(orig));
    uLong origLen = (uLong)sizeof(orig);
    uLong bound = compressBound(origLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)orig, origLen, 6);
    Bytef *decompBuf = (Bytef *)malloc((size_t)origLen);
    memset(decompBuf, 0, (size_t)origLen);
    uLongf decompLen = (uLongf)origLen;
    uLong srcLen = (uLong)compLen;

    // step 2: Initialize streams
    inflateInit2_(&infStrm, 15, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate: decompress with uncompress2 and compute checksum
    uncompress2(decompBuf, &decompLen, compBuf, &srcLen);
    uLong checksum = adler32(0UL, decompBuf, (uInt)decompLen);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(decompBuf);
    (void)checksum;
    return 66; // API sequence test completed successfully
}