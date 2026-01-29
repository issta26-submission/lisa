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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress data
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 2: Write compressed data to a gzip file and read a byte back
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, (voidpc)compBuf, (unsigned int)compLen);
    gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int firstByte = gzgetc_(gzr);
    (void)firstByte;

    // step 3: Initialize inflate, configure and call reset/cleanup APIs
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateResetKeep(&infStrm);
    inflateBackEnd(&infStrm);

    // step 4: Finalize streams, close gz and free buffers
    inflateEnd(&infStrm);
    gzclose(gzr);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}