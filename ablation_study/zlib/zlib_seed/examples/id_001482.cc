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
//<ID> 1482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: version/compress/gzclose/crc32_combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_part1 = crc32(0L, src, (uInt)(srcLen / 2));
    uLong crc_part2 = crc32(0L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong crc_combined_parts = crc32_combine(crc_part1, crc_part2, (off_t)(srcLen - (srcLen / 2)));
    uLong bound = compressBound(srcLen);
    Bytef *compressed = new Bytef[(size_t)bound];
    uLongf compressedLen = (uLongf)bound;

    // step 2: Setup (compress and checksum compressed output)
    int rc_compress = compress(compressed, &compressedLen, src, srcLen);
    uLong crc_compressed = crc32(0L, compressed, (uInt)compressedLen);
    uLong crc_final = crc32_combine(crc_combined_parts, crc_compressed, (off_t)compressedLen);

    // step 3: Core operations (write original to gz file, validate by uncompressing)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzclose = gzclose(gzf);
    Bytef *recovered = new Bytef[(size_t)srcLen];
    memset(recovered, 0, (size_t)srcLen);
    uLongf recoveredLen = (uLongf)srcLen;
    int rc_uncompress = uncompress(recovered, &recoveredLen, compressed, (uLong)compressedLen);

    // step 4: Cleanup
    delete [] src;
    delete [] compressed;
    delete [] recovered;
    (void)version;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined_parts;
    (void)crc_compressed;
    (void)crc_final;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_uncompress;
    // API sequence test completed successfully
    return 66;
}