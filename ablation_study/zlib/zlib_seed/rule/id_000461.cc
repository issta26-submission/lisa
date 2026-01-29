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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress data
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 2: Write compressed data to a gzip file and close it
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    gzclose(gz);

    // step 3: Initialize inflateBack, reset keep state, and teardown with inflateBackEnd
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&backStrm);
    inflateBackEnd(&backStrm);

    // step 4: Reopen the gzip file for reading, read one byte with gzgetc_, and cleanup
    gzFile gz2 = gzopen("test_zlib_api_sequence.gz", "rb");
    int ch = gzgetc_(gz2);
    (void)ch;
    gzclose(gz2);
    free(compBuf);
    free(window);
    return 66;
    // API sequence test completed successfully
}