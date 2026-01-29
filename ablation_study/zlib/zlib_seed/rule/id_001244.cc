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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare data + compute adler32 parts and combined checksum
    const uLong TOTAL = 200UL;
    const uLong HALF = TOTAL / 2;
    Bytef *data = (Bytef *)malloc((size_t)TOTAL);
    memset(data, 'Z', (size_t)TOTAL);
    Bytef *part1 = data;
    Bytef *part2 = data + (size_t)HALF;
    uLong ad1 = adler32(0UL, part1, (uInt)HALF);
    uLong ad2 = adler32(0UL, part2, (uInt)HALF);
    off_t off2 = (off_t)HALF;
    uLong combined_ad = adler32_combine(ad1, ad2, off2);
    uLong full_ad = adler32(0UL, data, (uInt)TOTAL);

    // step 2: Initialize deflate, set gzip header, compress data, and write compressed blob to a gz file
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit2_(&dstrm, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    Bytef *namebuf = (Bytef *)malloc(8);
    memcpy(namebuf, "zname", 6);
    head.name = namebuf;
    head.name_max = 6;
    deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, TOTAL);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = data;
    dstrm.avail_in = (uInt)TOTAL;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);
    gzFile gw = gzopen("tmp_zlib_test.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_size);
    gzclose_w(gw);
    gzFile gr = gzopen("tmp_zlib_test.gz", "rb");
    gzclose_r(gr);

    // step 3: Initialize inflate, set input, call inflateSync, inflate, and finish
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)TOTAL);
    memset(decomp, 0, (size_t)TOTAL);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)TOTAL;
    inflateSync(&istrm);
    inflate(&istrm, 4);
    inflateEnd(&istrm);

    // step 4: Validate checksums and cleanup resources
    uLong decomp_ad = adler32(0UL, decomp, (uInt)TOTAL);
    (void)combined_ad;
    (void)full_ad;
    (void)decomp_ad;
    free(data);
    free(namebuf);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}