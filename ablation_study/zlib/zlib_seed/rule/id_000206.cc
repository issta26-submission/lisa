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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup / Configure
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    uLongf outLen = (uLongf)inputLen;
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)outLen;
    inflateSync(&inf_strm);

    // step 3: Core operations (decompress with uncompress2, then recompress with deflate)
    uncompress2(outBuf, &outLen, compBuf, &compLen);
    uLong recompressBound = compressBound((uLong)outLen);
    Bytef *reCompBuf = (Bytef *)malloc((size_t)recompressBound);
    def_strm.next_in = outBuf;
    def_strm.avail_in = (uInt)outLen;
    def_strm.next_out = reCompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)reCompBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outBuf);
    free(reCompBuf);

    // API sequence test completed successfully
    return 66;
}