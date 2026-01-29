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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char msg[] = "Sample data for zlib usage";
    const Bytef * buffer = (const Bytef *)msg;
    z_size_t buflen = (z_size_t)(sizeof(msg) - 1);
    uLong crc_initial = 0UL;
    uLong crc_result;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzFile gz2 = gzdopen(1, "wb");
    int rc_write1;
    int rc_printf;
    int rc_puts;
    off64_t offset1;
    off64_t offset2;
    int rc_close1;
    int rc_close2;

    // step 2: Setup / Configure (compute checksum and write data)
    crc_result = crc32_z(crc_initial, buffer, buflen);
    rc_write1 = gzwrite(gz1, (voidpc)buffer, (unsigned int)buflen);
    rc_printf = gzprintf(gz1, " CRC=%lu\n", (unsigned long)crc_result);
    rc_puts = gzputs(gz2, "gzdopen output line\n");

    // step 3: Operate / Validate (obtain offsets reflecting writes)
    offset1 = gzoffset64(gz1);
    offset2 = gzoffset64(gz2);

    // step 4: Cleanup
    rc_close1 = gzclose(gz1);
    rc_close2 = gzclose(gz2);

    // API sequence test completed successfully
    (void)crc_result;
    (void)rc_write1;
    (void)rc_printf;
    (void)rc_puts;
    (void)offset1;
    (void)offset2;
    (void)rc_close1;
    (void)rc_close2;
    (void)scratch;
    return 66;
}