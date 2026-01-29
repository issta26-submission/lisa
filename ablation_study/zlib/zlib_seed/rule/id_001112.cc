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
//<ID> 1112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 128UL;
    Bytef *in = (Bytef *)malloc((size_t)LEN);
    memset(in, 'A', (size_t)LEN);
    uLong crc1 = crc32(0UL, in, (uInt)LEN);
    uLong crc2 = crc32_z(0UL, in, (z_size_t)LEN);

    // step 2: Initialize inflate stream and header structure
    const char *ver = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = in;
    istrm.avail_in = (uInt)LEN;
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&istrm, &head);

    // step 3: Perform an inflate operation (drive data through stream)
    Bytef *out = (Bytef *)malloc((size_t)LEN);
    memset(out, 0, (size_t)LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)LEN;
    inflate(&istrm, 0);

    // step 4: Cleanup and finalize
    inflateEnd(&istrm);
    free(out);
    free(in);
    (void)crc1;
    (void)crc2;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}