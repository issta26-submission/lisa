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
//<ID> 163
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
    z_stream inf_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char input[128];
    unsigned char out1[256];
    unsigned char out2[256];
    memset(input, 'A', sizeof(input));
    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize inflater and create a copy)
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)out1;
    inf_strm.avail_out = (uInt)sizeof(out1);
    inflateCopy(&inf_copy, &inf_strm);
    inf_copy.next_out = (Bytef *)out2;
    inf_copy.avail_out = (uInt)sizeof(out2);

    // step 3: Operate (inflate with both original and copied stream, then deflate-reset-keep on the decompressed data)
    inflate(&inf_strm, 0);
    inflate(&inf_copy, 0);
    uLong decompressedSize = (uLong)inf_strm.total_out;
    uLong compBound2 = compressBound(decompressedSize);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)out1;
    def_strm.avail_in = (uInt)decompressedSize;
    def_strm.next_out = compBuf2;
    def_strm.avail_out = (uInt)compBound2;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf2, (unsigned int)def_strm.total_out);
    gzrewind(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&inf_copy);
    gzclose(gz);
    free(compBuf);
    free(compBuf2);

    // API sequence test completed successfully
    return 66;
}