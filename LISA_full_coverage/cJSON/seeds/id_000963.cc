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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"arr\":[true],\"obj\":{\"keep\":\"val\"}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON *raw_item = cJSON_AddRawToObject(obj, "rawdata", "{\"x\":1}");

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(obj, "raw_is", raw_flag);
    cJSON_DeleteItemFromObjectCaseSensitive(obj, "keep");

    // step 4: Validate & Cleanup
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}