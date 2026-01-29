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
//<ID> 281
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 4);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    unsigned char outBuf1[256];
    unsigned char outBuf2[256];
    memset(outBuf1, 0, sizeof(outBuf1));
    memset(outBuf2, 0, sizeof(outBuf2));
    inf_strm.next_out = outBuf1;
    inf_strm.avail_out = (uInt)sizeof(outBuf1);
    inflateCopy(&inf_copy, &inf_strm);
    inf_copy.next_out = outBuf2;
    inf_copy.avail_out = (uInt)sizeof(outBuf2);
    deflateReset(&def_strm);
    inflate(&inf_strm, 0);
    inflate(&inf_copy, 0);
    outBuf1[(size_t)inf_strm.total_out] = '\0';
    outBuf2[(size_t)inf_copy.total_out] = '\0';

    // step 3: Validate (write decompressed output to a gzip file)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)outBuf1);

    // step 4: Cleanup
    gzclose(gz);
    inflateEnd(&inf_copy);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    return 66;
    // API sequence test completed successfully
}