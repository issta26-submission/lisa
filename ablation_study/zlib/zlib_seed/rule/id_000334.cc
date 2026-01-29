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
//<ID> 334
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
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (deflate + compress2)
    uLong bound = deflateBound(&dstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compLenUL = dstrm.total_out;
    deflateEnd(&dstrm);
    uLongf comp2Len = (uLongf)compressBound(inputLen);
    Bytef * comp2Buf = (Bytef *)malloc((size_t)comp2Len);
    memset(comp2Buf, 0, (size_t)comp2Len);
    compress2(comp2Buf, &comp2Len, (const Bytef *)input, inputLen, 6);

    // step 3: Configure & Operate (inflate), Query & Validate
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLenUL;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflateUndermine(&istrm, 1);
    inflate(&istrm, 0);
    unsigned long codesUsed = inflateCodesUsed(&istrm);
    (void)codesUsed;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLenUL);

    // step 4: Cleanup
    gzclose(gz);
    inflateEnd(&istrm);
    free(compBuf);
    free(comp2Buf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}