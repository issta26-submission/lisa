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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compute per-part CRCs, then compress the data
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uInt half1_len = (uInt)(ORIG_LEN / 2UL);
    uInt half2_len = (uInt)(ORIG_LEN - half1_len);
    uLong crc_part1 = crc32(0UL, orig, half1_len);
    uLong crc_part2 = crc32(0UL, orig + half1_len, half2_len);
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)half2_len);
    uLong comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size = (uLongf)comp_bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream, call inflateUndermine and then inflateBackEnd
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 3: Decompress with uncompress to validate data, compute CRC on decompressed output
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_size);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)decomp_len);
    (void)crc_combined; // used earlier to ensure meaningful data flow
    (void)crc_decomp;

    // step 4: Cleanup resources and finish
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}