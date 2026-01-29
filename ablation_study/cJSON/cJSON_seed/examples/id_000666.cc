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
//<ID> 666
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int initial_nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateIntArray(initial_nums, 4);
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", arr_dup);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snap_len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snap_len + 1);
    memset(buffer, 0, snap_len + 1);
    memcpy(buffer, snapshot, snap_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_vals = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    int parsed_size = cJSON_GetArraySize(parsed_vals);
    cJSON *first_item = cJSON_GetArrayItem(parsed_vals, 0);
    double first_number = cJSON_GetNumberValue(first_item);
    (void)version;
    (void)parsed_size;
    (void)first_number;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}