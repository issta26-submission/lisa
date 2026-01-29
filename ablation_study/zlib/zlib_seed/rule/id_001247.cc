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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute adler32 checksums and combined checksum
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong ad1 = adler32(0UL, src, (uInt)SRC_LEN);
    const uLong PART_LEN = 64UL;
    Bytef *part = (Bytef *)malloc((size_t)PART_LEN);
    memcpy(part, src + (SRC_LEN - PART_LEN), (size_t)PART_LEN);
    uLong ad2 = adler32(0UL, part, (uInt)PART_LEN);
    off_t part_off = (off_t)PART_LEN;
    uLong combined_ad = adler32_combine(ad1, ad2, part_off);

    // step 2: Initialize and configure deflate stream, attach a gzip header, and compress
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.text = 1;
    hdr.time = 0;
    hdr.xflags = 0;
    hdr.os = 255;
    hdr.extra = NULL;
    hdr.extra_len = 0;
    hdr.extra_max = 0;
    hdr.name = NULL;
    hdr.name_max = 0;
    hdr.comment = NULL;
    hdr.comm_max = 0;
    hdr.hcrc = 0;
    hdr.done = 0;
    deflateSetHeader(&dstrm, &hdr);
    uLong bound = deflateBound(&dstrm, SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, provide compressed data, call inflateSync, then decompress
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)SRC_LEN);
    memset(decomp, 0, (size_t)SRC_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)SRC_LEN;
    inflateSync(&istrm);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Use a gzFile to write compressed data and then clean up all resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_size);
    gzclose_r(gf);
    free(src);
    free(part);
    free(comp);
    free(decomp);
    (void)combined_ad;
    // API sequence test completed successfully
    return 66;
}