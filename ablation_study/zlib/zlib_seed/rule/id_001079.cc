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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and checksums
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 64UL;
    const uLong TOTAL = LEN1 + LEN2;
    Bytef *part1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *part2 = (Bytef *)malloc((size_t)LEN2);
    memset(part1, 'A', (size_t)LEN1);
    memset(part2, 'B', (size_t)LEN2);
    Bytef *combined = (Bytef *)malloc((size_t)TOTAL);
    memset(combined, 0, (size_t)TOTAL);
    memcpy(combined, part1, (size_t)LEN1);
    memcpy(combined + LEN1, part2, (size_t)LEN2);
    uLong ad1 = adler32(0UL, part1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, part2, (uInt)LEN2);
    uLong combined_adler = adler32_combine64(ad1, ad2, (off64_t)LEN2);
    const char *ver = zlibVersion();

    // step 2: Compress combined data using deflate stream
    uLong bound = compressBound(TOTAL);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = combined;
    dstrm.avail_in = (uInt)TOTAL;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate the compressed data and exercise inflateBack APIs
    Bytef *decomp = (Bytef *)malloc((size_t)TOTAL);
    memset(decomp, 0, (size_t)TOTAL);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)TOTAL;
    inflate(&istrm, 0);
    inflateEnd(&istrm);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);
    free(window);

    // step 4: Write and read a gzip file, finalize and cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, combined, (unsigned int)TOTAL);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)TOTAL);
    memset(readbuf, 0, (size_t)TOTAL);
    gzread(gr, readbuf, (unsigned int)TOTAL);
    gzclose_r(gr);
    free(readbuf);
    free(part1);
    free(part2);
    free(combined);
    free(comp);
    free(decomp);
    (void)combined_adler;
    (void)ver;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}