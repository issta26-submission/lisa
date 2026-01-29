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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and input preparation
    const char payload[] = "zlib API sequence payload demonstrating deflate -> inflate with dictionary and validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound <= (uLong)~0u ? bound : (uLong)~0u);

    // step 2: Configure deflate parameters
    int rc_deflate_params = deflateParams(&dstrm, 6, 0);

    // step 3: Operate: perform deflate, then prepare inflate, set dictionary and validate
    int rc_deflate_call = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);

    Bytef dict[4];
    dict[0] = src_buf[0];
    dict[1] = src_buf[1];
    dict[2] = src_buf[2];
    dict[3] = src_buf[3];
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dict, (uInt)4);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate_call = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_call;
    (void)comp_size;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_validate;
    (void)rc_inflate_call;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}