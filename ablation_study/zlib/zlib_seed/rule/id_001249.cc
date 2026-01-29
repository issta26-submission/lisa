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
//<ID> 1249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data chunks and compute Adler-32 checksums, then combine them
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 64UL;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong ad1 = adler32(0UL, chunk1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, chunk2, (uInt)LEN2);
    off_t off_len2 = (off_t)LEN2;
    uLong combined_ad = adler32_combine(ad1, ad2, off_len2);

    // step 2: Initialize and configure a deflate stream and attach a gzip header
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    const char *name_str = "test_zlib_api_sequence";
    size_t name_len = strlen(name_str) + 1;
    header.name = (Bytef *)malloc(name_len);
    memcpy(header.name, name_str, name_len);
    header.name_max = (uInt)name_len;
    deflateSetHeader(&dstrm, &header);

    // step 3: Initialize inflate stream and call inflateSync, then create a gzip file, write and reopen for read and close with gzclose_r
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSync(&istrm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, chunk1, (unsigned int)LEN1);
    gzwrite(gw, chunk2, (unsigned int)LEN2);
    gzflush(gw, 0);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclose_r(gr);

    // step 4: Cleanup streams, allocated memory and finalize
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(header.name);
    free(chunk1);
    free(chunk2);
    (void)combined_ad;
    // API sequence test completed successfully
    return 66;
}