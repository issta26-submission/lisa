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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflateBack -> header";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize and configure deflate stream and gz header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    const char head_name[] = "zlib_test_name";
    head.name = (Bytef *)malloc((size_t)sizeof(head_name));
    memcpy(head.name, head_name, sizeof(head_name));
    head.name_max = (uInt)sizeof(head_name);
    const char head_comment[] = "zlib test comment";
    head.comment = (Bytef *)malloc((size_t)sizeof(head_comment));
    memcpy(head.comment, head_comment, sizeof(head_comment));
    head.comm_max = (uInt)sizeof(head_comment);
    head.text = 1;
    head.time = (uLong)0;
    head.hcrc = 0;
    int rc_set_hdr = deflateSetHeader(&dstrm, &head);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate-back, attach compressed buffer and query sync point
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_back_end = inflateBackEnd(&istrm);

    // step 4: Cleanup and finalize
    free(compBuf);
    free(window);
    free(head.name);
    free(head.comment);
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_set_hdr;
    (void)rc_def_end;
    (void)rc_inf_back_init;
    (void)rc_sync;
    (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}