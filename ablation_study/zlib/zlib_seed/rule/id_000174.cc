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
//<ID> 174
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
    unsigned char output[512];
    memset(input, 'X', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize inflater and deflater, prime inflater, reset-keep deflater, open gz)
    inflateInit2_(&inf_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflatePrime(&inf_strm, 1, 1);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Operate (inflate compressed buffer, check sync point, deflate the result)
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)output;
    inf_strm.avail_out = (uInt)sizeof(output);
    inflate(&inf_strm, 0);
    inflateSyncPoint(&inf_strm);

    uLong decompSize = inf_strm.total_out;
    uLong recompressBound = compressBound(decompSize);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = (Bytef *)output;
    def_strm.avail_in = (uInt)decompSize;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)decompSize);
    (void)checksum;
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}