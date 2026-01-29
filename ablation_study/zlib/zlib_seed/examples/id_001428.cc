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
//<ID> 1428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload demonstrating deflateInit_/deflateCopy/deflateGetDictionary/gzopen/gzwrite";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    Bytef dict[] = { 0x10, 0x20, 0x30, 0x40, 0x50 };
    const uInt dictLen = (uInt)(sizeof(dict) / sizeof(dict[0]));
    Bytef *dictOut = new Bytef[(size_t)dictLen];
    memset(dictOut, 0, (size_t)dictLen);

    // step 2: Setup - initialize deflate stream, set a dictionary and prepare copy
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    z_stream dcopy;
    memset(&dcopy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dcopy, &dstrm);
    uInt dictLenOut = dictLen;
    int rc_deflate_get_dict = deflateGetDictionary(&dcopy, dictOut, &dictLenOut);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations - perform deflation and write original data via gz
    int rc_deflate = deflate(&dstrm, 0);
    uLong compLen = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_copy_end = deflateEnd(&dcopy);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] dictOut;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_set_dict;
    (void)rc_deflate_copy;
    (void)rc_deflate_get_dict;
    (void)bound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_end;
    (void)rc_deflate_copy_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}