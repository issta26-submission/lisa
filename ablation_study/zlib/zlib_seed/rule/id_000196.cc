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
//<ID> 196
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
    unsigned char input[512];
    unsigned char out_buf[8192];
    memset(input, 'A', sizeof(input));
    memset(out_buf, 0, sizeof(out_buf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup (initialize deflater and prepare for compression)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);
    uLong compressedLen = def_strm.total_out;

    // step 3: Initialize inflater, reconfigure, and decompress
    inflateInit2_(&inf_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateReset2(&inf_strm, 15);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compressedLen;
    inf_strm.next_out = (Bytef *)out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    inflate(&inf_strm, 0);
    inflateBackEnd(&inf_strm);

    // step 4: Validate (write compressed data to gz) & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compressedLen);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}