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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and write a gz file
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)inputLen);
    gzclose(gz);

    // step 2: Reopen gz file and read a single byte using gzgetc_
    gzFile gz2 = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gz2);
    (void)c;
    gzclose(gz2);

    // step 3: Initialize inflate, decompress the compressed buffer
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);

    // step 4: Reset keep, finalize inflate back-end and cleanup
    inflateResetKeep(&infStrm);
    inflateBackEnd(&infStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}