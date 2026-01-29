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
//<ID> 1111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 128UL;
    Bytef *buf = (Bytef *)malloc((size_t)LEN);
    memset(buf, 'A', (size_t)LEN);
    uLong crc_initial = crc32(0UL, buf, (uInt)LEN);
    uLong crc_initial_z = crc32_z(0UL, buf, (z_size_t)LEN);

    // step 2: Initialize inflate stream and header structure
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));

    // step 3: Configure/operate/validate: associate header retrieval with stream and exercise CRC APIs
    inflateGetHeader(&istrm, &head);
    uLong crc_after_header = crc32(crc_initial, buf, (uInt)LEN);
    uLong crc_after_header_z = crc32_z(crc_initial_z, buf, (z_size_t)LEN);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(buf);
    // API sequence test completed successfully
    (void)crc_initial;
    (void)crc_initial_z;
    (void)crc_after_header;
    (void)crc_after_header_z;
    (void)ver;
    return 66;
}