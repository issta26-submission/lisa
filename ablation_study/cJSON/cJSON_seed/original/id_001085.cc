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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[1,2,3],\"name\":\"example\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *disabled = cJSON_AddFalseToObject(root, "disabled");
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(nested, "raw", raw_item);

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int arr_size = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    cJSON_AddNumberToObject(root, "raw_flag", (double)raw_flag);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}