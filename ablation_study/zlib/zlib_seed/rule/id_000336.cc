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
//<ID> 336
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
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong dbound = deflateBound(&dstrm, inputLen);
    Bytef * defBuf = (Bytef *)malloc((size_t)dbound);
    memset(defBuf, 0, (size_t)dbound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)dbound;
    uLong cbound = compressBound(inputLen);
    Bytef * comp2Buf = (Bytef *)malloc((size_t)cbound);
    memset(comp2Buf, 0, (size_t)cbound);
    uLongf comp2Len = (uLongf)cbound;
    compress2(comp2Buf, &comp2Len, (const Bytef *)input, inputLen, 6);

    // step 3: Operate & Validate
    deflate(&dstrm, 4);
    uLong defCompLen = dstrm.total_out;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_in = defBuf;
    istrm.avail_in = (uInt)defCompLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflateUndermine(&istrm, 1);
    inflate(&istrm, 0);
    unsigned long used = inflateCodesUsed(&istrm);
    (void)used;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp2Buf, (unsigned int)comp2Len);

    // step 4: Cleanup
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(defBuf);
    free(comp2Buf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}