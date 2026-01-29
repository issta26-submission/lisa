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
//<ID> 205
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

    // step 2: Setup (initialize deflater, allocate buffers)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);

    // step 3: Operate (write compressed data, decompress with uncompress2, use inflateSync)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);
    uLongf destLen = (uLongf)inputLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)inputLen);
    memset(destBuf, 0, (size_t)inputLen);
    uLong sourceLenVar = (uLong)def_strm.total_out;
    uncompress2(destBuf, &destLen, compBuf, &sourceLenVar);
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)sourceLenVar;
    inflateSync(&inf_strm);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(destBuf);

    // API sequence test completed successfully
    return 66;
}