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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream strm_deflate;
    z_stream strm_inflate;
    gzFile gz_file_handle = Z_NULL;

    const char *test_filename = "test_zlib_data.gz";

    const uInt SOURCE_LEN = 128;
    Bytef source_buf[SOURCE_LEN];
    uLongf compressed_buf_len = SOURCE_LEN * 2;
    Bytef compressed_buf[SOURCE_LEN * 2];
    uLongf uncompressed_buf_len = SOURCE_LEN;
    Bytef uncompressed_buf[SOURCE_LEN];

    Bytef gz_write_buf[64];
    Bytef gz_read_buf[32];

    Bytef checksum_data_buf[50];
    uLong adler_val = 1L;
    uLong crc_val = 0L;

    int ret;

    // Step 2: Setup (Initialization)
    memset(&strm_deflate, 0, sizeof(z_stream));
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));

    memset(&strm_inflate, 0, sizeof(z_stream));
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    ret = inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(z_stream));

    memset(source_buf, 'A', SOURCE_LEN);
    memset(compressed_buf, 0, compressed_buf_len);
    memset(uncompressed_buf, 0, uncompressed_buf_len);

    memset(gz_write_buf, 'B', sizeof(gz_write_buf));
    memset(gz_read_buf, 0, sizeof(gz_read_buf));

    memset(checksum_data_buf, 'C', sizeof(checksum_data_buf));

    // Step 3: Core Operations - Compression, Checksums, Deflate Prime
    ret = compress(compressed_buf, &compressed_buf_len, source_buf, SOURCE_LEN);

    adler_val = adler32(adler_val, checksum_data_buf, sizeof(checksum_data_buf));
    adler_val = adler32(adler_val, Z_NULL, 0); // Edge case: NULL buffer, zero length

    crc_val = crc32_z(crc_val, checksum_data_buf, (z_size_t)sizeof(checksum_data_buf));
    crc_val = crc32_z(crc_val, Z_NULL, (z_size_t)0); // Edge case: NULL buffer, zero length

    ret = deflatePrime(&strm_deflate, 8, 0xAB);

    // Step 4: GZ File Write Operations
    gz_file_handle = gzopen(test_filename, "wb");
    ret = gzwrite(gz_file_handle, (voidpc)gz_write_buf, sizeof(gz_write_buf));
    ret = gzclose(gz_file_handle);
    gz_file_handle = Z_NULL;

    // Step 5: GZ File Read Operations
    gz_file_handle = gzopen(test_filename, "rb");
    ret = gzread(gz_file_handle, (voidp)gz_read_buf, sizeof(gz_read_buf));
    ret = gzread(gz_file_handle, (voidp)gz_read_buf, 0); // Edge case: zero length read
    ret = gzclose(gz_file_handle);
    gz_file_handle = Z_NULL;

    // Step 6: Cleanup
    ret = deflateEnd(&strm_deflate);
    ret = inflateEnd(&strm_inflate);

    remove(test_filename);

    printf("API sequence test completed successfully\n");

    return 66;
}