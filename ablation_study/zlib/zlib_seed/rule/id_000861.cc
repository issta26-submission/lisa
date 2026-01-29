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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUFLEN = 256UL;
    Bytef *buf = (Bytef *)malloc((size_t)BUFLEN);
    memset(buf, 'Z', (size_t)BUFLEN);
    uInt first_half = (uInt)(BUFLEN / 2UL);
    uLong crc_first = crc32(0UL, buf, first_half);
    uLong crc_combined = crc32(crc_first, buf + first_half, (uInt)(BUFLEN - first_half));
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    strm.next_in = buf + first_half;
    strm.avail_in = (uInt)(BUFLEN - first_half);

    // step 2: Initialize inflate stream and exercise reset-keep
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    deflateResetKeep(&strm);

    // step 3: Operate using gzdopen and compute an additional CRC for written data
    gzFile gf = gzdopen(1, "wb");
    gzputs(gf, "zlib");
    uLong crc_written = crc32(0UL, (const Bytef *)"zlib", (uInt)5);
    gzclose(gf);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(buf);
    (void)ver;
    (void)crc_first;
    (void)crc_combined;
    (void)crc_written;
    // API sequence test completed successfully
    return 66;
}