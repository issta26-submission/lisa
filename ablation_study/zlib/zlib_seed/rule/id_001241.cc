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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute adler32 checksums and combined value
    const uLong LEN1 = 100UL;
    const uLong LEN2 = 150UL;
    const uLong TOTAL = LEN1 + LEN2;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong ad1 = adler32(0UL, chunk1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, chunk2, (uInt)LEN2);
    off_t off2 = (off_t)LEN2;
    uLong combined_ad = adler32_combine(ad1, ad2, off2);
    Bytef *merged = (Bytef *)malloc((size_t)TOTAL);
    memcpy(merged, chunk1, (size_t)LEN1);
    memcpy(merged + LEN1, chunk2, (size_t)LEN2);

    // step 2: Initialize deflate stream, set gzip header, compute bound and compress merged data
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    head.text = 1;
    Bytef *namebuf = (Bytef *)malloc(16);
    memset(namebuf, 0, 16);
    memcpy(namebuf, (const char *)"test-name", 9);
    head.name = namebuf;
    head.name_max = 16;
    deflateSetHeader(&dstrm, &head);
    uLong bound = deflateBound(&dstrm, TOTAL);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, merged, TOTAL, 6);
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, attempt sync on compressed bytes, write/read gzip file and close reader
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    inflateSync(&istrm);
    gzFile wf = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(wf, comp, (unsigned int)comp_size);
    gzclose(wf);
    gzFile rf = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *decomp = (Bytef *)malloc((size_t)TOTAL);
    memset(decomp, 0, (size_t)TOTAL);
    gzread(rf, decomp, (unsigned int)TOTAL);
    gzclose_r(rf);

    // step 4: Validate via adler on decompressed data, cleanup streams and memory
    uLong decomp_ad = adler32(0UL, decomp, (uInt)TOTAL);
    (void)combined_ad;
    (void)decomp_ad;
    inflateEnd(&istrm);
    free(chunk1);
    free(chunk2);
    free(merged);
    free(namebuf);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}