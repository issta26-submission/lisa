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
//<ID> 560
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef *out1 = (Bytef *)malloc((size_t)inputLen);
    memset(out1, 0, (size_t)inputLen);
    Bytef *out2 = (Bytef *)malloc((size_t)inputLen);
    memset(out2, 0, (size_t)inputLen);
    Bytef *out3 = (Bytef *)malloc((size_t)inputLen);
    memset(out3, 0, (size_t)inputLen);

    // step 2: Use uncompress and uncompress2 to decompress the compressed buffer
    uLongf out1Len = (uLongf)inputLen;
    uncompress(out1, &out1Len, compBuf, (uLong)compLen);
    uLongf out2Len = (uLongf)inputLen;
    uLong srcLen = (uLong)compLen;
    uncompress2(out2, &out2Len, compBuf, &srcLen);

    // step 3: Initialize inflate, set dictionary, perform inflate, then end inflate
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateSetDictionary(&infStrm, (const Bytef *)input, (uInt)8);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = out3;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    inflateEnd(&infStrm);

    // step 4: Cleanup
    free(compBuf);
    free(out1);
    free(out2);
    free(out3);

    return 66;
    // API sequence test completed successfully
}