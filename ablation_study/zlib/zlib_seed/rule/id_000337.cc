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
//<ID> 337
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
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure streams and buffers
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;

    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, inputLen);
    Bytef * defBuf = (Bytef *)malloc((size_t)defBound);
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;

    // step 3: Operate & Validate
    deflate(&dstrm, 4);
    unsigned long codes_after_deflate = inflateCodesUsed(&dstrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    inflate(&istrm, 0);
    inflateUndermine(&istrm, 1);
    unsigned long codes_after_inflate = inflateCodesUsed(&istrm);
    (void)codes_after_deflate;
    (void)codes_after_inflate;

    // step 4: Cleanup
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(defBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}