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
//<ID> 335
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    uLongf comp1Bound = (uLongf)compressBound(inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)comp1Bound);
    memset(compBuf1, 0, (size_t)comp1Bound);
    uLongf comp1Len = comp1Bound;
    compress2(compBuf1, &comp1Len, (const Bytef *)input, inputLen, 6);

    // step 2: Configure & Operate (deflate)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong comp2Bound = deflateBound(&dstrm, inputLen);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)comp2Bound);
    memset(compBuf2, 0, (size_t)comp2Bound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)comp2Bound;
    deflate(&dstrm, 4);
    uLong comp2Len = dstrm.total_out;

    // step 3: Configure & Operate (inflate) and validation calls
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf1;
    istrm.avail_in = (uInt)comp1Len;
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);
    unsigned long used_codes = inflateCodesUsed(&istrm);
    (void)used_codes;
    inflateUndermine(&istrm, 1);

    // step 4: Finalize & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)(comp2Len & 0xFF));
    gzputc(gz, (int)((comp2Len >> 8) & 0xFF));
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf1);
    free(compBuf2);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}