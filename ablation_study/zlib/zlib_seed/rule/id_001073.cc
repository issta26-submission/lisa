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
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and checksums
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 64UL;
    Bytef *data1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *data2 = (Bytef *)malloc((size_t)LEN2);
    memset(data1, 'A', (size_t)LEN1);
    memset(data2, 'B', (size_t)LEN2);
    uLong ad1 = adler32(0L, data1, (uInt)LEN1);
    uLong ad2 = adler32(0L, data2, (uInt)LEN2);
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)LEN2);

    // step 2: Initialize inflate-back stream with a sliding window
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    backstrm.adler = combined_ad;
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Tear down inflate-back and finalize inflate stream
    inflateBackEnd(&backstrm);
    inflateEnd(&backstrm);

    // step 4: Use gz file API for a small operation and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclose_r(gf);
    free(data1);
    free(data2);
    free(window);
    // API sequence test completed successfully
    return 66;
}