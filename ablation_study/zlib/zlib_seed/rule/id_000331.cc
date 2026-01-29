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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Compress raw data with compress2 and open gz file
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)(compLen & 0xFF));
    gzputc(gz, (int)((compLen >> 8) & 0xFF));

    // step 3: Initialize deflate, run deflate on compressed buffer, and call inflate-related APIs
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLong dbound = deflateBound(&dstrm, (uLong)compLen);
    Bytef * defBuf = (Bytef *)malloc((size_t)dbound);
    memset(defBuf, 0, (size_t)dbound);
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)dbound;
    deflate(&dstrm, 4);

    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&istrm);
    (void)codes_used;
    inflateUndermine(&istrm, 1);

    // step 4: Cleanup
    gzclose(gz);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(defBuf);
    // API sequence test completed successfully
    return 66;
}