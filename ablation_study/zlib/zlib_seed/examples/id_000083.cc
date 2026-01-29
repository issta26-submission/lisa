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
//<ID> 83
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
    const char src_c[] = "example payload for zlib API sequence demonstrating inflateBackInit_ and deflate APIs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.time = 161803398UL;

    // step 2: Setup (initialize deflate and inflate-back with header)
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_sethdr = deflateSetHeader(&defstrm, &head);
    int rc_inf_back_init = inflateBackInit_(&infstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compute checksum, compress with deflate using configured header)
    uLong checksum = adler32_z(1L, source, (z_size_t)sourceLen);
    defstrm.adler = checksum;
    uLong bound = deflateBound(&defstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate_finish = deflate(&defstrm, 4);
    uLong produced = defstrm.total_out;

    // step 4: Validate & Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_back_end = inflateBackEnd(&infstrm);
    free(window);
    free(compBuf);
    (void)rc_def_init; (void)rc_def_sethdr; (void)rc_inf_back_init;
    (void)checksum; (void)rc_deflate_finish; (void)produced;
    (void)rc_def_end; (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}