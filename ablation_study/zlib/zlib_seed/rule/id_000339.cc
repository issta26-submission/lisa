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
//<ID> 339
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

    uLong bound1 = compressBound(inputLen);
    Bytef * comp2Buf = (Bytef *)malloc((size_t)bound1);
    memset(comp2Buf, 0, (size_t)bound1);
    uLongf comp2DestLen = (uLongf)bound1;
    compress2(comp2Buf, &comp2DestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Configure & Operate (deflate using deflate API)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    Bytef * defOut = (Bytef *)malloc((size_t)bound1);
    memset(defOut, 0, (size_t)bound1);
    dstrm.next_in = comp2Buf;
    dstrm.avail_in = (uInt)comp2DestLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)bound1;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, use inflateUndermine & inspect with inflateCodesUsed
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = defOut;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflateUndermine(&istrm, 1);
    unsigned long codesUsed = inflateCodesUsed(&istrm);
    (void)codesUsed;
    inflateEnd(&istrm);

    // step 4: Write compressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp2Buf, (unsigned int)comp2DestLen);
    gzclose(gz);
    free(comp2Buf);
    free(defOut);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}