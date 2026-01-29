#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"sensor\":\"temp\",\"ok\":true,\"value\":23.5}";
    const char *version = NULL;
    const char *parse_end_ptr = NULL;
    cJSON *root = NULL;
    cJSON *value_item = NULL;
    char *unformatted = NULL;
    char *prebuf = NULL;
    int buf_len = 128;
    cJSON_bool prealloc_ok = 0;
    double value_double = 0.0;
    int value_int = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_ParseWithLengthOpts(json, (size_t)strlen(json), &parse_end_ptr, 1);
    value_item = cJSON_GetObjectItem(root, "value");
    value_double = cJSON_GetNumberValue(value_item);
    value_int = (int)value_double;

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted(root);
    prebuf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prebuf, 0, (size_t)buf_len);
    prealloc_ok = cJSON_PrintPreallocated(root, prebuf, buf_len, 0);
    prebuf[0] = version ? version[0] : '\0';
    prebuf[1] = (char)('0' + (value_int % 10));
    prebuf[2] = *parse_end_ptr;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}