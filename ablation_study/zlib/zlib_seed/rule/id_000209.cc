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
//<ID> 209
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
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong destCap = inputLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destCap);
    uLong destLen = destCap;

    // step 2: Setup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations
    uncompress2(destBuf, &destLen, compBuf, &compLen);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = destBuf;
    inf_strm.avail_out = (uInt)destCap;
    inflate(&inf_strm, 0);
    inflateSync(&inf_strm);
    uLong recompressBound = compressBound((uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    def_strm.next_in = destBuf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);

    // step 4: Validate & Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);
    free(destBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}