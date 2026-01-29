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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream copyStrm;
    z_stream backStrm;
    memset(&istrm, 0, sizeof(istrm));
    memset(&copyStrm, 0, sizeof(copyStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[512];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Initialize inflate, operate, and copy stream
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);
    memset(&copyStrm, 0, sizeof(copyStrm));
    inflateCopy(&copyStrm, &istrm);
    inflateEnd(&copyStrm);
    inflateEnd(&istrm);

    // step 3: Initialize inflateBack, operate, and teardown
    unsigned char window[64];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    backStrm.next_in = compBuf;
    backStrm.avail_in = (uInt)compDestLen;
    backStrm.next_out = outBuf2;
    backStrm.avail_out = (uInt)inputLen;
    inflateBack(&backStrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&backStrm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzflush(gz, 2);
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    free(outBuf2);
    return 66;
    // API sequence test completed successfully
}