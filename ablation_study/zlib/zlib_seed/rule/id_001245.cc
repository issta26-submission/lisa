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
//<ID> 1245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute Adler-32 checksums, then combine
    const uLong TOTAL_LEN = 256UL;
    const uLong HALF_LEN = TOTAL_LEN / 2UL;
    Bytef *data = (Bytef *)malloc((size_t)TOTAL_LEN);
    Bytef *part1 = (Bytef *)malloc((size_t)HALF_LEN);
    Bytef *part2 = (Bytef *)malloc((size_t)HALF_LEN);
    memset(data, 'Z', (size_t)TOTAL_LEN);
    memcpy(part1, data, (size_t)HALF_LEN);
    memcpy(part2, data + HALF_LEN, (size_t)HALF_LEN);
    uLong ad1 = adler32(0UL, part1, (uInt)HALF_LEN);
    uLong ad2 = adler32(0UL, part2, (uInt)HALF_LEN);
    off_t off2 = (off_t)HALF_LEN;
    uLong combined_ad = adler32_combine(ad1, ad2, off2);
    (void)combined_ad;

    // step 2: Initialize and configure deflate stream with a gzip header
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    Bytef *hname = (Bytef *)malloc(8);
    memcpy(hname, "demo", 5);
    header.name = hname;
    header.name_max = 8U;
    header.comment = NULL;
    header.comment = (Bytef *)malloc(12);
    memcpy(header.comment, "cmt", 4);
    header.comm_max = 12U;
    deflateSetHeader(&dstrm, &header);

    // step 3: Operate - compress using deflate and then decompress using inflate; call inflateSync
    dstrm.next_in = data;
    dstrm.avail_in = (uInt)TOTAL_LEN;
    uLong bound = deflateBound(&dstrm, TOTAL_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 31, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)TOTAL_LEN);
    memset(decomp, 0, (size_t)TOTAL_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)TOTAL_LEN;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateSync(&istrm);

    // step 4: Validate-like operation and cleanup: write compressed data to a gzip file and close; end streams and free memory
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    if (gf != NULL) {
        gzwrite(gf, comp, (unsigned int)comp_size);
        gzclose_r(gf);
    }
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(data);
    free(part1);
    free(part2);
    free(comp);
    free(decomp);
    free(hname);
    free(header.comment);
    // API sequence test completed successfully
    return 66;
}