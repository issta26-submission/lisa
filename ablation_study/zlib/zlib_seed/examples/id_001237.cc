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
//<ID> 1237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char part1[] = "Hello_zlib_part1";
    const char part2[] = "World_zlib_part2";
    const uLong len1 = (uLong)(sizeof(part1) - 1);
    const uLong len2 = (uLong)(sizeof(part2) - 1);
    const uLong total_len = len1 + len2;
    const char *version = zlibVersion();
    Bytef *src_buf = new unsigned char[(size_t)total_len];
    memset(src_buf, 0, (size_t)total_len);
    memcpy(src_buf, part1, (size_t)len1);
    memcpy(src_buf + len1, part2, (size_t)len2);
    uLong crc1 = crc32(0UL, (const Bytef *)part1, (uInt)len1);
    uLong crc2 = crc32(0UL, (const Bytef *)part2, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);

    // step 2: Setup (compress source into buffer and prepare for uncompress)
    uLong comp_bound = compressBound(total_len);
    Bytef *comp_buf = new unsigned char[(size_t)comp_bound];
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, total_len, 6);
    uLong source_len_for_uncompress = (uLong)comp_len_f;
    uLongf dest_len_f = (uLongf)total_len;
    Bytef *dest_buf = new unsigned char[(size_t)total_len];
    memset(dest_buf, 0, (size_t)total_len);

    // step 3: Core operations (uncompress, setup stream for validation, gz operations)
    int rc_uncompress = uncompress2(dest_buf, &dest_len_f, comp_buf, &source_len_for_uncompress);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)source_len_for_uncompress;
    istrm.next_out = dest_buf;
    istrm.avail_out = (uInt)dest_len_f;
    istrm.adler = combined_crc;
    istrm.total_in = (uLong)source_len_for_uncompress;
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)dest_buf, (unsigned int)(dest_len_f ? dest_len_f : 0));
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalize
    delete [] src_buf;
    delete [] comp_buf;
    delete [] dest_buf;
    (void)version;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)rc_compress;
    (void)rc_uncompress;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}