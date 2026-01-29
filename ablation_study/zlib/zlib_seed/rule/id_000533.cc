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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateReset(&defStrm);

    // step 2: Configure inflate stream
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&infStrm);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSync(&infStrm);

    // step 3: Operate
    inflate(&infStrm, Z_FINISH);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzputc(gz, (int)input[0]);
    gzclose(gz);

    // step 4: Validate & Cleanup
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}