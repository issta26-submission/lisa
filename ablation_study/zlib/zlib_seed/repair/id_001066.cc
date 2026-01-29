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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef payload[] = "Example payload for zlib sequence testing.";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong half_len = payload_len / 2;
    uLong crc_first = crc32(0UL, payload, (uInt)half_len);
    uLong crc_second = crc32(0UL, payload + half_len, (uInt)(payload_len - half_len));
    off_t second_len_off = (off_t)(payload_len - half_len);
    uLong combined_crc = crc32_combine(crc_first, crc_second, second_len_off);

    // step 2: Configure / Operate (write payload to a gz file and seek)
    gzFile gf = gzopen("tmp_zlib_sequence_test.gz", "wb");
    int gzwrite_ret = gzwrite(gf, (voidpc)payload, (unsigned int)payload_len);
    off_t new_pos = gzseek(gf, 0, 0);

    // step 3: Inflate-related initialization and safety configuration
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&inf, 1);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    int gzclose_ret = gzclose(gf);
    (void)version;
    (void)crc_first;
    (void)crc_second;
    (void)combined_crc;
    (void)gzwrite_ret;
    (void)new_pos;
    (void)inf_init_ret;
    (void)undermine_ret;
    (void)inf_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}