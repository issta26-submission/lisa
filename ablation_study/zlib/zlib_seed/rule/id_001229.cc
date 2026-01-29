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
//<ID> 1229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare two input chunks and compute individual CRCs, then combine CRCs
    const uLong LEN1 = 64UL;
    const uLong LEN2 = 128UL;
    const uLong TOTAL = LEN1 + LEN2;
    Bytef *chunk1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *chunk2 = (Bytef *)malloc((size_t)LEN2);
    memset(chunk1, 'A', (size_t)LEN1);
    memset(chunk2, 'B', (size_t)LEN2);
    uLong crc1 = crc32(0UL, chunk1, (uInt)LEN1);
    uLong crc2 = crc32(0UL, chunk2, (uInt)LEN2);
    off_t len2_off = (off_t)LEN2;
    uLong combined_crc = crc32_combine(crc1, crc2, len2_off);

    // step 2: Merge chunks, compress merged data, and then decompress
    Bytef *merged = (Bytef *)malloc((size_t)TOTAL);
    memcpy(merged, chunk1, (size_t)LEN1);
    memcpy(merged + LEN1, chunk2, (size_t)LEN2);
    uLong bound = compressBound(TOTAL);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, merged, TOTAL, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)TOTAL);
    memset(decomp, 0, (size_t)TOTAL);
    uLongf decomp_len = TOTAL;
    uncompress(decomp, &decomp_len, comp, comp_size);
    uLong decomp_crc = crc32(0UL, decomp, (uInt)decomp_len);

    // step 3: Initialize inflate stream, adjust undermine setting, call inflateBackEnd, then cleanup inflate
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    // step 4: Cleanup allocated memory and finish
    free(chunk1);
    free(chunk2);
    free(merged);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}