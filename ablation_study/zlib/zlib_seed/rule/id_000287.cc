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
//<ID> 287
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
    z_stream copy_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (compress then reset compressor)
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 4);
    deflateReset(&def_strm);

    // step 3: Configure & Operate (prepare inflate, copy state, decompress into buffers)
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);
    inflateCopy(&copy_strm, &inf_strm);
    unsigned char outBufCopy[256];
    memset(outBufCopy, 0, sizeof(outBufCopy));
    copy_strm.next_out = outBufCopy;
    copy_strm.avail_out = (uInt)sizeof(outBufCopy);
    inflate(&copy_strm, 0);

    // step 4: Validate & Cleanup
    outBufCopy[sizeof(outBufCopy) - 1] = '\0';
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)outBufCopy);
    gzclose(gz);
    inflateEnd(&inf_strm);
    inflateEnd(&copy_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    return 66;
    // API sequence test completed successfully
}