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
//<ID> 392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize input and checksum
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Initialize deflate stream and prepare output buffer
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    uLong boundOut = deflateBound(&dstrm, inputLen);
    Bytef * defOut = (Bytef *)malloc((size_t)boundOut);
    memset(defOut, 0, (size_t)boundOut);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)boundOut;
    dstrm.adler = checksum;

    // step 3: Operate (perform deflate) and write/read via gz APIs
    deflate(&dstrm, 4); // Z_FINISH == 4
    unsigned int compressedSize = (unsigned int)dstrm.total_out;
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, defOut, compressedSize);
    gzclose(gzw);
    Bytef * readBack = (Bytef *)malloc((size_t)inputLen);
    memset(readBack, 0, (size_t)inputLen);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzread(gzr, readBack, (unsigned int)inputLen);
    gzclose_r(gzr);

    // step 4: Cleanup and teardown
    deflateEnd(&dstrm);
    free(defOut);
    free(readBack);
    return 66;
    // API sequence test completed successfully
}