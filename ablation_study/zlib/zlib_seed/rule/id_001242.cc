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
//<ID> 1242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute adler32 checksums and combine them
    const uLong LEN1 = 100UL;
    const uLong LEN2 = 150UL;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong a1 = adler32(0UL, chunk1, (uInt)LEN1);
    uLong a2 = adler32(0UL, chunk2, (uInt)LEN2);
    off_t len2_off = (off_t)LEN2;
    uLong combined_adler = adler32_combine(a1, a2, len2_off);

    // step 2: Initialize and configure a deflate stream and set a gzip header
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    Bytef *nameBuf = (Bytef *)malloc(6);
    memcpy(nameBuf, "zlib", 5);
    nameBuf[5] = 0;
    head.name = nameBuf;
    head.name_max = 6U;
    head.time = combined_adler;
    deflateSetHeader(&dstrm, &head);

    // step 3: Initialize an inflate stream and call inflateSync
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSync(&istrm);

    // step 4: Use gz API to open and close a gzip file and perform cleanup
    gzFile gf = gzopen("test_zlib_api.tmp", "wb");
    gzclose_r(gf);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(chunk1);
    free(chunk2);
    free(nameBuf);
    // API sequence test completed successfully
    return 66;
}