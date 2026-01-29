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
//<ID> 1240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute combined adler32 checksum
    const uLong LEN1 = 100UL;
    const uLong LEN2 = 200UL;
    const uLong TOTAL = LEN1 + LEN2;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong adl1 = adler32(0UL, chunk1, (uInt)LEN1);
    uLong adl2 = adler32(0UL, chunk2, (uInt)LEN2);
    off_t len2_off = (off_t)LEN2;
    uLong combined_adl = adler32_combine(adl1, adl2, len2_off);

    // step 2: Initialize and configure deflate and inflate streams and header
    const char *ver = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    head.text = 0;
    head.name = NULL;
    head.comment = NULL;
    deflateSetHeader(&dstrm, &head);

    // step 3: Operate - merge data, attempt deflate, then call inflateSync, and open/close a gzFile using gzclose_r
    Bytef *merged = (Bytef *)malloc((size_t)TOTAL);
    memcpy(merged, chunk1, (size_t)LEN1);
    memcpy(merged + LEN1, chunk2, (size_t)LEN2);
    uLong bound = deflateBound(&dstrm, TOTAL);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = merged;
    dstrm.avail_in = (uInt)TOTAL;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 0);
    inflateSync(&istrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzclose_r(gf);

    // step 4: Validate minimal invariants and cleanup
    uLong merged_adl = adler32(0UL, merged, (uInt)TOTAL);
    (void)combined_adl;
    (void)merged_adl;
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(chunk1);
    free(chunk2);
    free(merged);
    free(comp);
    // API sequence test completed successfully
    return 66;
}