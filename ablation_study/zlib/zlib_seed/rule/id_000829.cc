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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong TOTAL = 256UL;
    const uLong HALF = 128UL;
    Bytef *buf = (Bytef *)malloc((size_t)TOTAL);
    memset(buf, 'Z', (size_t)TOTAL);
    uLong crc_first = crc32(0UL, buf, (uInt)HALF);
    uLong crc_second = crc32(0UL, buf + HALF, (uInt)(TOTAL - HALF));
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(TOTAL - HALF));

    // step 2: Initialize inflate stream and prime bit buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)TOTAL;
    inflatePrime(&istrm, 3, 5);

    // step 3: Gzip file operations: open, write, flush, close
    gzFile gz = gzopen64("test_zlib_api_seq.gz", "wb");
    gzwrite(gz, (voidpc)buf, (unsigned int)TOTAL);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(buf);
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}