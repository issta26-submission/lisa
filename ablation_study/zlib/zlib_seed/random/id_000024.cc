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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src_c[] = "Example data for zlib gz writing and crc32_z test";
    const Bytef * source = (const Bytef *)src_c;
    z_size_t sourceLen = (z_size_t)(sizeof(src_c) - 1);
    uLong crc_initial = 0;
    uLong crc_after = 0;
    gzFile file1;
    gzFile file2;
    off64_t offset1;
    off64_t offset2;
    int rc_close1;
    int rc_close2;
    int rc_write1;
    int rc_write2;

    // step 2: Setup/Configure
    crc_initial = crc32_z(0, source, sourceLen);
    file1 = gzopen("test_zlib_api_sequence.gz", "wb");
    file2 = gzdopen(1, "wb");

    // step 3: Operate/Validate
    rc_write1 = gzputs(file1, src_c);
    rc_write2 = gzputs(file2, src_c);
    offset1 = gzoffset64(file1);
    offset2 = gzoffset64(file2);
    crc_after = crc32_z(crc_initial, source, sourceLen);

    // step 4: Cleanup
    rc_close1 = gzclose(file1);
    rc_close2 = gzclose(file2);

    // API sequence test completed successfully
    (void)crc_initial;
    (void)crc_after;
    (void)offset1;
    (void)offset2;
    (void)rc_close1;
    (void)rc_close2;
    (void)rc_write1;
    (void)rc_write2;
    return 66;
}