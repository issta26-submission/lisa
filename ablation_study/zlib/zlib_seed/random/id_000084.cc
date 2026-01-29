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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequence: deflate with header and inflateBack init";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    char *nameBuf = (char *)malloc(16);
    char *commBuf = (char *)malloc(32);
    memset(nameBuf, 0, 16);
    memset(commBuf, 0, 32);
    memcpy(nameBuf, "test_name", 9);
    memcpy(commBuf, "test_comment", 12);
    head.name = (Bytef *)nameBuf;
    head.name_max = (uInt)strlen(nameBuf);
    head.comment = (Bytef *)commBuf;
    head.comm_max = (uInt)strlen(commBuf);
    head.hcrc = 0;

    // step 2: Setup (initialize streams and configure deflate header)
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_hdr = deflateSetHeader(&defstrm, &head);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    int rc_inf_back_init = inflateBackInit_(&infstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compute checksum and perform a deflate pass)
    uLong checksum = adler32_z(1UL, source, (z_size_t)sourceLen);
    int rc_deflate = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    (void)checksum;
    (void)rc_deflate;

    // step 4: Cleanup & finalize
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_back_end = inflateBackEnd(&infstrm);
    free(compBuf);
    free(nameBuf);
    free(commBuf);
    free(window);
    (void)rc_def_init; (void)rc_set_hdr; (void)rc_inf_back_init; (void)rc_def_end; (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}