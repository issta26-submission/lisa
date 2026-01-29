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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm_back;
    z_stream strm_stream;
    memset(&strm_back, 0, sizeof(strm_back));
    memset(&strm_stream, 0, sizeof(strm_stream));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compDestLen = (uLongf)bound;
    compress2(compBuf, &compDestLen, (const Bytef *)input, inputLen, 6);

    // step 2: Inflate using inflateBack (callback pointers as NULL as a valid usage here)
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    unsigned char out_back[256];
    memset(out_back, 0, sizeof(out_back));
    inflateBackInit_(&strm_back, 15, window, zlibVersion(), (int)sizeof(z_stream));
    strm_back.next_in = compBuf;
    strm_back.avail_in = (uInt)compDestLen;
    strm_back.next_out = out_back;
    strm_back.avail_out = (uInt)sizeof(out_back);
    inflateBack(&strm_back, (in_func)0, (void *)0, (out_func)0, (void *)0);
    inflateBackEnd(&strm_back);

    // step 3: Inflate using stream-based inflate for validation
    Bytef * out_stream = (Bytef *)malloc((size_t)inputLen);
    memset(out_stream, 0, (size_t)inputLen);
    inflateInit_(&strm_stream, zlibVersion(), (int)sizeof(z_stream));
    strm_stream.next_in = compBuf;
    strm_stream.avail_in = (uInt)compDestLen;
    strm_stream.next_out = out_stream;
    strm_stream.avail_out = (uInt)inputLen;
    inflate(&strm_stream, 0);
    inflateEnd(&strm_stream);

    // step 4: Write compressed data and checksum to a gz file, then cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compDestLen);
    gzwrite(gz, (voidpc)&checksum, (unsigned int)sizeof(checksum));
    gzflush(gz, 0);
    gzclose(gz);
    free(compBuf);
    free(out_stream);
    return 66; // API sequence test completed successfully
}