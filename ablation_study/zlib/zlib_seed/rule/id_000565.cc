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
//<ID> 565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Initialize inflate stream and set dictionary
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)8);

    // step 3: Decompress using uncompress and uncompress2
    Bytef * out1 = (Bytef *)malloc((size_t)inputLen);
    memset(out1, 0, (size_t)inputLen);
    uLongf decompLen1 = (uLongf)inputLen;
    uncompress(out1, &decompLen1, compBuf, compLen);
    Bytef * out2 = (Bytef *)malloc((size_t)inputLen);
    memset(out2, 0, (size_t)inputLen);
    uLongf decompLen2 = (uLongf)inputLen;
    uLong srcLenForUncompress2 = (uLong)compLen;
    uncompress2(out2, &decompLen2, compBuf, &srcLenForUncompress2);

    // step 4: Configure stream output pointer, finalize and cleanup
    infStrm.next_out = out1;
    infStrm.avail_out = (uInt)decompLen1;
    inflateEnd(&infStrm);
    free(compBuf);
    free(out1);
    free(out2);

    return 66;
    // API sequence test completed successfully
}