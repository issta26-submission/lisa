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
//<ID> 964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\": [true], \"remove_me\": \"temp\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "refs", false_item);
    cJSON_AddItemReferenceToArray(items, false_item);
    cJSON *raw_added = cJSON_AddRawToObject(root, "rawdata", "{\"nested\":42}");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "rawdata");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}