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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&backstrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequence: deflate + inflateBackInit + headers + checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *nameBuf = (Bytef *)malloc(16);
    memset(compBuf, 0, (size_t)bound);
    memset(nameBuf, 0, 16);

    // step 2: Setup (initialize deflate and inflate-back, configure header)
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    memcpy(nameBuf, "testname", 8);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.name = nameBuf;
    head.name_max = 16;
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_head = deflateSetHeader(&defstrm, &head);
    int rc_back_init = inflateBackInit_(&backstrm, 15, (unsigned char *)NULL, version, (int)sizeof(z_stream));

    // step 3: Core operations (compress once and compute adler checksum)
    int rc_def = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    uLong written = defstrm.total_out;
    uLong checksum_input = adler32_z(1UL, source, (z_size_t)sourceLen);
    uLong checksum_comp = adler32_z(0UL, compBuf, (z_size_t)written);

    // step 4: Cleanup & finalize
    int rc_def_end = deflateEnd(&defstrm);
    int rc_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(nameBuf);
    (void)rc_def_init; (void)rc_set_head; (void)rc_back_init; (void)rc_def; (void)checksum_input; (void)checksum_comp; (void)rc_def_end; (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}