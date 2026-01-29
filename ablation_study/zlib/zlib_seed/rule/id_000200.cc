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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&def_strm, 0, sizeof(def_strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong outSize = inputLen * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)outSize);
    memset(outBuf, 0, (size_t)outSize);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)outSize;
    inflate(&inf_strm, 0);
    inflateSync(&inf_strm);

    // step 3: Core operations
    uLong defBufSize = deflateBound(&def_strm, (uLong)inf_strm.total_out);
    Bytef *defBuf = (Bytef *)malloc((size_t)defBufSize);
    memset(defBuf, 0, (size_t)defBufSize);
    def_strm.next_in = outBuf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = defBuf;
    def_strm.avail_out = (uInt)defBufSize;
    deflate(&def_strm, 0);
    uLongf finalBufLen = (uLongf)(inputLen * 2);
    Bytef *finalBuf = (Bytef *)malloc((size_t)finalBufLen);
    memset(finalBuf, 0, (size_t)finalBufLen);
    uLong sourceLen = (uLong)def_strm.total_out;
    uncompress2(finalBuf, &finalBufLen, defBuf, &sourceLen);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, defBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outBuf);
    free(defBuf);
    free(finalBuf);

    // API sequence test completed successfully
    return 66;
}