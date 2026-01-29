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
//<ID> 208
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    uLongf destLen = (uLongf)inputLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destLen);
    memset(destBuf, 0, (size_t)destLen);

    // step 3: Core operations
    uncompress2(destBuf, &destLen, compBuf, &compLen);
    inf_strm.next_out = destBuf;
    inf_strm.avail_out = (uInt)destLen;
    inflateSync(&inf_strm);
    def_strm.next_in = destBuf;
    def_strm.avail_in = (uInt)destLen;
    uLong recompressBound = deflateBound(&def_strm, (uLong)destLen);
    Bytef *recompressBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompressBuf, 0, (size_t)recompressBound);
    def_strm.next_out = recompressBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, recompressBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(destBuf);
    free(recompressBuf);

    // API sequence test completed successfully
    return 66;
}