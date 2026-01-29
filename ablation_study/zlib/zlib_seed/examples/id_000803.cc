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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute compressed bound and allocate buffers
    const char src[] = "zlib API sequence payload to compress, write to gz, then read back and inspect positions";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // step 2: Initialize deflate stream, provide input and output buffers, and perform compression
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&strm, 4); // Z_FINISH == 4
    uLong produced = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Persist compressed data to a gzip file, probe positions, then reopen and read back with gzread
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)produced);
    int rc_gz_flush = gzflush(gf, 0);
    off_t pos_after_write = gztell(gf);
    off64_t pos64_after_write = gzoffset64(gf);
    int rc_gz_close_write = gzclose(gf);
    gzFile gf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(readBuf, 0, (size_t)compAlloc);
    int rc_gz_read = gzread(gf_read, readBuf, (unsigned int)compAlloc);
    int rc_gz_close_read = gzclose(gf_read);

    // step 4: Validate sizes, cleanup and finish
    uLong written_bytes = produced;
    uLong read_bytes = (uLong)((rc_gz_read >= 0) ? rc_gz_read : 0);
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_flush;
    (void)pos_after_write;
    (void)pos64_after_write;
    (void)rc_gz_close_write;
    (void)rc_gz_read;
    (void)rc_gz_close_read;
    (void)written_bytes;
    (void)read_bytes;
    // API sequence test completed successfully
    return 66;
}