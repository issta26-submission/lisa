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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const Bytef message[] = "ZLIB test data for crc and gzip";
    z_size_t msg_len = (z_size_t)(sizeof(message) - 1);
    uLong crc = crc32_z(0, message, msg_len);
    const char *ver = zlibVersion();
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    int fd = open("test_zlib_api_sequence_fd.gz", O_RDWR | O_CREAT | O_TRUNC, 0644);
    gzFile gz2 = gzdopen(fd, "wb");
    int wrote1;
    int wrote2;
    off64_t off1;
    off64_t off2;
    int rc_close1;
    int rc_close2;

    // step 2: Setup (write data to both gzip handles)
    wrote1 = gzwrite(gz1, (voidpc)message, (unsigned int)msg_len);
    wrote2 = gzwrite(gz2, (voidpc)message, (unsigned int)msg_len);

    // step 3: Operate (write crc value bytes to both, to make data flow meaningful)
    uLong crc_copy = crc;
    gzwrite(gz1, (voidpc)&crc_copy, (unsigned int)sizeof(crc_copy));
    gzwrite(gz2, (voidpc)&crc_copy, (unsigned int)sizeof(crc_copy));

    // step 4: Validate -> Cleanup (query offsets and close)
    off1 = gzoffset64(gz1);
    off2 = gzoffset64(gz2);
    rc_close1 = gzclose(gz1);
    rc_close2 = gzclose(gz2);

    // API sequence test completed successfully
    (void)ver;
    (void)wrote1;
    (void)wrote2;
    (void)off1;
    (void)off2;
    (void)rc_close1;
    (void)rc_close2;
    (void)crc;

    return 66;
}