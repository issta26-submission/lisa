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
//<ID> 333
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBufSize = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    uLongf compLen = compBufSize;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Inflate Initialize, Configure & Operate (decompress) and Validate
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);
    unsigned long codes_used = inflateCodesUsed(&istrm);
    (void)codes_used;
    inflateUndermine(&istrm, 1);

    // step 3: Deflate Initialize, Configure & Operate (re-compress) and write to gz
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = outBuf;
    dstrm.avail_in = (uInt)inputLen;
    uLong defCompSize = deflateBound(&dstrm, inputLen);
    Bytef * compBuf2 = (Bytef *)malloc((size_t)defCompSize);
    memset(compBuf2, 0, (size_t)defCompSize);
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)defCompSize;
    deflate(&dstrm, 4);
    uLong finalCompLen = dstrm.total_out;
    gzwrite(gz, compBuf2, (unsigned int)finalCompLen);

    // step 4: Cleanup
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    free(compBuf2);
    return 66; // API sequence test completed successfully
}