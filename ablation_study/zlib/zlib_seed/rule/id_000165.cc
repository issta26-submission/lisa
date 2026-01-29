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
//<ID> 165
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
    unsigned char out1[256];
    unsigned char out2[256];
    memset(input, 'A', sizeof(input));
    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);

    // step 2: Setup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = out1;
    inf_strm.avail_out = (uInt)sizeof(out1);
    inflateCopy(&copy_strm, &inf_strm);
    copy_strm.next_out = out2;
    copy_strm.avail_out = (uInt)sizeof(out2);

    // step 3: Core operations
    inflate(&inf_strm, 0);
    inflate(&copy_strm, 0);

    // step 4: Validate & Cleanup
    uLong checksum1 = adler32(0L, out1, (uInt)inf_strm.total_out);
    uLong checksum2 = adler32(0L, out2, (uInt)copy_strm.total_out);
    (void)checksum1;
    (void)checksum2;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)out1, (unsigned int)inf_strm.total_out);
    gzrewind(gz);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&copy_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}