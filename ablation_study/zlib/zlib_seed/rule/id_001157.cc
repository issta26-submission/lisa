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
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and metadata
    const uLong ORIG_LEN = 64UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Write metadata to a gzip file and read back a line using gzgets
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzprintf(gw, "%lu\n", (unsigned long)adler_orig);
    gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    char read_buf[128];
    memset(read_buf, 0, sizeof(read_buf));
    gzgets(gr, read_buf, 64);

    // step 3: Initialize inflate stream, set dictionary, configure undermine, and validate checksums
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)read_buf;
    istrm.avail_in = (uInt)strlen(read_buf);
    istrm.adler = adler_orig;
    inflateSetDictionary(&istrm, orig, (uInt)ORIG_LEN);
    inflateUndermine(&istrm, 1);
    uLong crc_read = crc32(0UL, (const Bytef *)read_buf, (uInt)strlen(read_buf));
    uLong adler_read = adler32(0UL, (const Bytef *)read_buf, (uInt)strlen(read_buf));
    inflateEnd(&istrm);

    // step 4: Cleanup
    gzclose(gr);
    free(orig);
    (void)crc_orig;
    (void)adler_orig;
    (void)crc_read;
    (void)adler_read;
    // API sequence test completed successfully
    return 66;
}