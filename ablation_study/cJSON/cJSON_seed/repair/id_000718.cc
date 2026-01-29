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
//<ID> 718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\": [1, 2, 3, 4], \"meta\": {\"id\": 42}}";
    size_t json_len = strlen(json);
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 2: Configure
    int extra_vals[3] = {100, 200, 300};
    cJSON *extra_array = cJSON_CreateIntArray(extra_vals, 3);
    cJSON_AddItemToObjectCS(root, "extra", extra_array);
    cJSON *detached_item = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToObjectCS(root, "detached_second", detached_item);

    // step 3: Operate and Validate
    int numbers_after = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "numbers_size_after_detach", (double)numbers_after);
    int extra_count = cJSON_GetArraySize(extra_array);
    cJSON_AddNumberToObject(root, "extra_size", (double)extra_count);
    cJSON *meta_id = cJSON_GetObjectItem(meta, "id");
    double meta_id_val = cJSON_GetNumberValue(meta_id);
    cJSON_AddNumberToObject(root, "meta_id_double", meta_id_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}