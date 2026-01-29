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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (Initialize)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    Bytef * defOutBuf = (Bytef *)malloc((size_t)bound);
    memset(defOutBuf, 0, (size_t)bound);

    // step 2: Initialize & Configure deflate stream (Configure)
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = defOutBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate & Validate (Operate -> write gz and compute checksum)
    deflate(&defStrm, 0);
    uLong deflatedLen = (uLong)((uLong)bound - (uLong)defStrm.avail_out);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, input, (unsigned int)inputLen);
    gzwrite(gz, compBuf, (unsigned int)compLen);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    (void)deflatedLen;
    (void)checksum;

    // step 4: Cleanup
    deflateEnd(&defStrm);
    gzclose(gz);
    free(compBuf);
    free(defOutBuf);

    return 66; // API sequence test completed successfully
}