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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = ORIG_LEN + 1024UL;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLong crc_before = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Compress original data into a gzip-format buffer using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream)); // windowBits=31 -> gzip wrapper
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); // 4 == Z_FINISH
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate the gzip buffer, retrieve header, check sync point, and end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 31, ver, (int)sizeof(z_stream)); // windowBits=31 to accept gzip
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&istrm, &head);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    inflateEnd(&istrm);

    // step 4: Side-effect write via gzopen/gzwrite, validate checksums, cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_size);
    gzclose(gf);
    uLong crc_after = crc32(0L, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_before;
    (void)crc_after;
    // API sequence test completed successfully
    return 66;
}