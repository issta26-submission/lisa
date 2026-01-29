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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    Bytef dict[32];
    memset(dict, 0x5A, sizeof(dict));
    uInt dictLen = (uInt)sizeof(dict);

    // step 2: Setup
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateSetDictionary(&defstrm, dict, dictLen);

    // step 3: Core operations
    deflateResetKeep(&defstrm);
    Bytef recDict[32];
    memset(recDict, 0, sizeof(recDict));
    uInt recDictLen = dictLen;
    deflateGetDictionary(&defstrm, recDict, &recDictLen);

    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "zlib test payload");
    gzseek64(gz, (off64_t)0, 0);
    gzclose(gz);

    // step 4: Validate & Cleanup
    deflateEnd(&defstrm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}