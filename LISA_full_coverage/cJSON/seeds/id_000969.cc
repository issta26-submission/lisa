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
//<ID> 969
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"arr\": [true], \"obj\": {}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON *raw_item = cJSON_AddRawToObject(obj, "raw", "{\"nested\":123}");
    cJSON_bool raw_is = cJSON_IsRaw(raw_item);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    cJSON_AddBoolToObject(root, "raw_was_raw", raw_is);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(obj, "raw");
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}