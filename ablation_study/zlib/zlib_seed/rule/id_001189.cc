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
//<ID> 1189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute adler checksums
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 64UL;
    Bytef *buf1 = (Bytef *)malloc((size_t)LEN1);
    memset(buf1, 'A', (size_t)LEN1);
    Bytef *buf2 = (Bytef *)malloc((size_t)LEN2);
    memset(buf2, 'B', (size_t)LEN2);
    const char *ver = zlibVersion();
    uLong ad1 = adler32(0UL, buf1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, buf2, (uInt)LEN2);
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)LEN2);

    // step 2: Initialize a deflate stream (will be cleaned up later)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 3: Compress buffer 1 and write compressed data to a gzip file using gzwrite
    uLong bound = compressBound(LEN1);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size_f = (uLongf)bound;
    compress2(comp, &comp_size_f, buf1, LEN1, 6);
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, comp, (unsigned int)comp_size_f);
    gzclose(gz);

    // step 4: Initialize inflate stream, adjust windowBits via inflateReset2, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset2(&istrm, 15);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(buf1);
    free(buf2);
    free(comp);
    // API sequence test completed successfully
    return 66;
}