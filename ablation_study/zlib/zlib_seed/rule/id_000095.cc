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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dsrc;
    z_stream dcopy;
    z_stream isrc;
    z_stream icopy;
    memset(&dsrc, 0, sizeof(dsrc));
    memset(&dcopy, 0, sizeof(dcopy));
    memset(&isrc, 0, sizeof(isrc));
    memset(&icopy, 0, sizeof(icopy));
    unsigned char input[128];
    unsigned char decompBuf[128];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong inputLen = (uLong)sizeof(input);
    uLong cb = compressBound(inputLen);
    unsigned char *compBuf = (unsigned char *)malloc((size_t)cb);
    memset(compBuf, 0, (size_t)cb);

    // step 2: Configure (deflate to create compressed data, copy deflate state)
    deflateInit_(&dsrc, 6, zlibVersion(), (int)sizeof(z_stream));
    dsrc.next_in = (Bytef *)input;
    dsrc.avail_in = (uInt)inputLen;
    dsrc.next_out = (Bytef *)compBuf;
    dsrc.avail_out = (uInt)cb;
    deflate(&dsrc, 0);
    unsigned int comp_len = (unsigned int)dsrc.total_out;
    deflateCopy(&dcopy, &dsrc);
    deflateEnd(&dcopy);

    // step 3: Operate & Validate (inflate, copy inflate state, validate/undermine, backend)
    inflateInit_(&isrc, zlibVersion(), (int)sizeof(z_stream));
    isrc.next_in = (Bytef *)compBuf;
    isrc.avail_in = (uInt)comp_len;
    isrc.next_out = (Bytef *)decompBuf;
    isrc.avail_out = (uInt)inputLen;
    inflate(&isrc, 0);
    inflateCopy(&icopy, &isrc);
    inflateValidate(&icopy, 0);
    inflateUndermine(&icopy, 1);
    inflateBackEnd(&icopy);

    // step 4: Cleanup & Output
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)decompBuf, (unsigned int)inputLen);
    gzflush(gz, 2);
    gzclose(gz);
    deflateEnd(&dsrc);
    inflateEnd(&isrc);
    inflateEnd(&icopy);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}