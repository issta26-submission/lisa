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
//<ID> 1431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddFalseToObject(meta, "active");

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    int count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddStringToObject(root, "note", "generated");

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_active = cJSON_GetObjectItem(got_meta, "active");
    cJSON_bool is_values_array = cJSON_IsArray(values);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *buffer = (char *)cJSON_malloc(out_len + 16);
    memset(buffer, 0, out_len + 16);
    buffer[0] = out ? out[0] : ' ';
    buffer[1] = (char)('0' + (count % 10));
    buffer[2] = (char)(is_values_array ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}