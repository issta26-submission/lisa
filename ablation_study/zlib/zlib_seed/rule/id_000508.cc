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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and produce a first-stage compressed buffer)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    uLong compLenU = (uLong)compLen;
    Bytef * defOutBuf = (Bytef *)malloc((size_t)bound);
    memset(defOutBuf, 0, (size_t)bound);

    // step 2: Initialize & Configure deflate stream (prepare to further deflate the compressed data)
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = compBuf;
    defStrm.avail_in = (uInt)compLenU;
    defStrm.next_out = defOutBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate (perform a deflate pass) and use gzwrite to write the original input to a gzip file
    deflate(&defStrm, 4);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, input, (unsigned int)inputLen);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&defStrm);
    free(compBuf);
    free(defOutBuf);

    return 66; // API sequence test completed successfully
}