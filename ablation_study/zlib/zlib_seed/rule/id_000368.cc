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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream cstrm;
    z_stream backstrm;
    z_stream copystrm;
    memset(&cstrm, 0, sizeof(cstrm));
    memset(&backstrm, 0, sizeof(backstrm));
    memset(&copystrm, 0, sizeof(copystrm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compileFlags = zlibCompileFlags();
    (void)compileFlags;
    deflateInit_(&cstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Compress
    uLong bound = deflateBound(&cstrm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    cstrm.next_in = (Bytef *)input;
    cstrm.avail_in = (uInt)inputLen;
    cstrm.next_out = compBuf;
    cstrm.avail_out = (uInt)bound;
    deflate(&cstrm, 4);
    uLong compLen = cstrm.total_out;
    deflateEnd(&cstrm);

    // step 3: Initialize inflateBack, copy stream, and inflateBack
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    backstrm.next_in = compBuf;
    backstrm.avail_in = (uInt)compLen;
    backstrm.next_out = outBuf;
    backstrm.avail_out = (uInt)inputLen;
    int copyRes = inflateCopy(&copystrm, &backstrm);
    (void)copyRes;
    inflateBack(&copystrm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&copystrm);
    inflateBackEnd(&backstrm);

    // step 4: Write gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzflush(gz, 0);
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}