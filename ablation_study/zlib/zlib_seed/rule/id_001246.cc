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
//<ID> 1246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data chunks and compute Adler-32 checksums, then combine them
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 256UL;
    const uLong TOTAL = LEN1 + LEN2;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong ad1 = adler32(0UL, chunk1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, chunk2, (uInt)LEN2);
    off_t len2_off = (off_t)LEN2;
    uLong combined_ad = adler32_combine(ad1, ad2, len2_off);

    // step 2: Initialize and configure deflate stream and gzip header
    gz_header *head = (gz_header *)malloc(sizeof(gz_header));
    memset(head, 0, sizeof(gz_header));
    head->text = 1;
    head->time = combined_ad;
    Bytef *namebuf = (Bytef *)malloc(6);
    memset(namebuf, 0, 6);
    memcpy(namebuf, "sname", 5);
    head->name = namebuf;
    head->name_max = 5;
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    deflateSetHeader(&dstrm, head);

    // step 3: Compress merged data, initialize inflate stream with compressed input, and call inflateSync
    Bytef *merged = (Bytef *)malloc((size_t)TOTAL);
    memcpy(merged, chunk1, (size_t)LEN1);
    memcpy(merged + LEN1, chunk2, (size_t)LEN2);
    uLong out_bound = compressBound(TOTAL);
    Bytef *outbuf = (Bytef *)malloc((size_t)out_bound);
    memset(outbuf, 0, (size_t)out_bound);
    dstrm.next_in = merged;
    dstrm.avail_in = (uInt)TOTAL;
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)out_bound;
    deflate(&dstrm, 4);
    uLong produced = out_bound - (uLong)dstrm.avail_out;
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = outbuf;
    istrm.avail_in = (uInt)produced;
    inflateSync(&istrm);

    // step 4: Write original merged data to a gzFile, perform cleanup of streams and memory
    gzFile gf = gzopen("test_zlib_api_sequence.tmp", "wb");
    gzwrite(gf, merged, (unsigned int)TOTAL);
    gzclose_r(gf);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(chunk1);
    free(chunk2);
    free(merged);
    free(outbuf);
    free(namebuf);
    free(head);
    // API sequence test completed successfully
    return 66;
}