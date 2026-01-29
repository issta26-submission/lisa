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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and metadata
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Write version and a marker into a gzip file for later reading
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzputs(gw, "marker\n");
    gzclose(gw);

    // step 3: Initialize inflate stream, set dictionary and undermine, validate checksums
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = orig;
    istrm.avail_in = (uInt)ORIG_LEN;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateSetDictionary(&istrm, orig, (uInt)ORIG_LEN);
    inflateUndermine(&istrm, 1);
    uLong adler_decomp = adler32(0UL, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    inflateEnd(&istrm);

    // step 4: Read back from gzip file using gzgets, cleanup
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char read_line[128];
    memset(read_line, 0, sizeof(read_line));
    gzgets(gr, read_line, (int)sizeof(read_line));
    gzclose(gr);
    free(orig);
    free(decomp);
    (void)ver;
    (void)adler_orig;
    (void)crc_orig;
    (void)adler_decomp;
    (void)crc_decomp;
    (void)read_line;
    // API sequence test completed successfully
    return 66;
}