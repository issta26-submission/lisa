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
//<ID> 177
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
    unsigned char input[128];
    unsigned char outbuf[512];
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inflatePrime(&inf_strm, 0, 0);
    inflateSyncPoint(&inf_strm);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Core operations
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    inflate(&inf_strm, 0);
    uLong decompressedSize = inf_strm.total_out;
    uLong recompressBound = compressBound(decompressedSize);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = (Bytef *)outbuf;
    def_strm.avail_in = (uInt)decompressedSize;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, outbuf, (uInt)decompressedSize);
    (void)checksum;
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}