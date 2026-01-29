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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&def_strm, 0, sizeof(def_strm));
    unsigned char input[512];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize deflater + inflater and adjust resets)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    inflateInit2_(&inf_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateReset2(&inf_strm, 15);

    // step 3: Core operations (inflate compressed data, then deflate result, write out)
    unsigned char out_buf[8192];
    memset(out_buf, 0, sizeof(out_buf));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    inflate(&inf_strm, 0);

    uLong recompressBound = compressBound((uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = (Bytef *)out_buf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    inflateBackEnd(&inf_strm);

    // step 4: Validate & cleanup
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}