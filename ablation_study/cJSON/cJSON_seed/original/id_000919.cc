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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"data\": [1, 2, 3], \"remove_me\": 10}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *new_arr = cJSON_AddArrayToObject(root, "new_array");
    cJSON *num42 = cJSON_CreateNumber(42);
    cJSON_AddItemToArray(new_arr, num42);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(new_arr, str);
    cJSON *data_item0 = cJSON_GetArrayItem(data, 0);
    double first_val = cJSON_GetNumberValue(data_item0);
    cJSON *num_from_data = cJSON_CreateNumber(first_val + 100.0);
    cJSON_AddItemToArray(new_arr, num_from_data);

    // step 3: Operate
    const char extra_json[] = "[{\"x\":1},{\"x\":2}]";
    size_t extra_len = sizeof(extra_json) - 1;
    cJSON *parsed_extra = cJSON_ParseWithLength(extra_json, extra_len);
    cJSON_AddItemToArray(new_arr, parsed_extra);
    cJSON *detached = cJSON_DetachItemFromObject(root, "remove_me");
    cJSON_Delete(detached);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}