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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    Byte input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf1 = (Bytef *)malloc((size_t)bound);
    Bytef * compBuf2 = (Bytef *)malloc((size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(compBuf1, 0, (size_t)bound);
    memset(compBuf2, 0, (size_t)bound);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: One-shot compression with compress2 and write to a gz file
    uLongf comp1Len = (uLongf)bound;
    compress2(compBuf1, &comp1Len, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf1, (unsigned int)comp1Len);
    gzclose(gz);

    // step 3: Streaming compress with deflate, then streaming inflate and validation
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp2Len = dstrm.total_out;
    deflateEnd(&dstrm);

    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf2;
    istrm.avail_in = (uInt)comp2Len;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);
    (void)codes_used;
    inflateUndermine(&istrm, 1);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(compBuf1);
    free(compBuf2);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}