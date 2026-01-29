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
//<ID> 88
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
    const char src_c[] = "Test payload for zlib sequence to exercise selected APIs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *inBuf = (Bytef *)malloc((size_t)sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.name = (Bytef *)malloc(32);
    memset(head.name, 0, 32);
    memcpy(head.name, "test_name", 9);
    head.name_max = 32;
    memset(inBuf, 0, (size_t)sourceLen);
    memset(compBuf, 0, (size_t)bound);
    memcpy(inBuf, source, (size_t)sourceLen);

    // step 2: Setup (initialize deflate and inflate-back state, compute adler)
    const char *version = zlibVersion();
    uLong initial_adler = adler32_z(1UL, inBuf, (z_size_t)sourceLen);
    defstrm.adler = initial_adler;
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&defstrm, &head);
    int rc_infback_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compress input with deflate and compute checksum of compressed output)
    defstrm.next_in = inBuf;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&defstrm, 0);
    uLong produced = defstrm.total_out;
    uLong comp_adler = adler32_z(0UL, compBuf, (z_size_t)produced);
    backstrm.adler = comp_adler;

    // step 4: Cleanup & finalize
    int rc_def_end = deflateEnd(&defstrm);
    int rc_back_end = inflateBackEnd(&backstrm);
    free(inBuf);
    free(compBuf);
    free(window);
    free(head.name);
    (void)rc_def_init; (void)rc_set_header; (void)rc_infback_init; (void)rc_deflate;
    (void)produced; (void)initial_adler; (void)comp_adler; (void)rc_def_end; (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}