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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers/streams
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLongf destLen = (uLongf)inputLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destLen);

    // step 2: Setup deflater and perform compression
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);
    uLong compLen = (uLong)def_strm.total_out;

    // step 3: Use gzopen to store compressed data, then decompress and use inflateSync on an inflate stream
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    gzclose(gz);
    uncompress2(destBuf, &destLen, compBuf, &compLen);
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = destBuf;
    inf_strm.avail_out = (uInt)destLen;
    inflateSync(&inf_strm);

    // step 4: Validate (implicit) & cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(destBuf);

    // API sequence test completed successfully
    return 66;
}