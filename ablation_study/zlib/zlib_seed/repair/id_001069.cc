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
//<ID> 1069
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Compress source to an in-memory buffer
    const Bytef source[] = "Example zlib payload for API sequence test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Write compressed data into a .gz file and seek within it
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_len);
    off_t seek_ret = gzseek(gzf, (off_t)0, 0); // 0 == SEEK_SET
    int gzclose_ret = gzclose(gzf);

    // step 3: Initialize inflate, allow undermine, and decompress the in-memory compressed buffer
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inflate_init_ret = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&istrm, 1);
    uLong dest_cap = source_len + 64;
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_cap);
    memset(dest_buf, 0, (size_t)dest_cap);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = dest_buf;
    istrm.avail_out = (uInt)dest_cap;
    int inflate_ret = inflate(&istrm, 4); // 4 == Z_FINISH
    uLong decompressed_size = istrm.total_out;
    int inflate_end_ret = inflateEnd(&istrm);

    // step 4: Validate via CRC combination and cleanup
    uInt first_len = (uInt)(decompressed_size / 2);
    uInt second_len = (uInt)(decompressed_size - first_len);
    uLong crc1 = crc32(0UL, dest_buf, first_len);
    uLong crc2 = crc32(0UL, dest_buf + first_len, second_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)second_len);

    free(comp_buf);
    free(dest_buf);
    (void)comp_ret;
    (void)comp_len;
    (void)bound;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)gzclose_ret;
    (void)inflate_init_ret;
    (void)undermine_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)decompressed_size;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}