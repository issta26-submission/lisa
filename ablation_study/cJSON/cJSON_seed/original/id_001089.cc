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
//<ID> 1089
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[10,20,30],\"raw\":\"some_raw_text\",\"info\":{\"a\":1}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *ver_item = cJSON_CreateString("v1.0");
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *disabled = cJSON_AddFalseToObject(meta, "enabled");

    // step 3: Operate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int numbers_count = cJSON_GetArraySize(numbers);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}