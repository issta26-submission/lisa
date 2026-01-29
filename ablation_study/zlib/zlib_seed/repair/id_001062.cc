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
//<ID> 1062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef buf1[] = "First chunk of data for gzwrite and CRC.";
    const Bytef buf2[] = "Second chunk of data for CRC combine.";
    uLong len1 = (uLong)(sizeof(buf1) - 1);
    uLong len2 = (uLong)(sizeof(buf2) - 1);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure / Operate (write compressed data to a gzip file and seek)
    gzFile gzf = gzopen("tmp_zlib_sequence.gz", "wb");
    int write_ret1 = gzwrite(gzf, buf1, (unsigned int)len1);
    int write_ret2 = gzwrite(gzf, buf2, (unsigned int)len2);
    off_t offset_end = gzseek(gzf, (off_t)0, 2);
    off_t offset_begin = gzseek(gzf, (off_t)0, 0);

    // step 3: Validate (compute CRCs and combine) and exercise undermine API
    uLong crc1 = crc32(0UL, buf1, (uInt)len1);
    uLong crc2 = crc32(0UL, buf2, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);
    int undermine_ret = inflateUndermine(&istrm, 1);

    // step 4: Cleanup
    int end_ret = inflateEnd(&istrm);
    int gzclose_ret = gzclose(gzf);
    (void)version;
    (void)init_ret;
    (void)write_ret1;
    (void)write_ret2;
    (void)offset_end;
    (void)offset_begin;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)undermine_ret;
    (void)end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}