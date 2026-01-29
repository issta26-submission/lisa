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
//<ID> 720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *str_one = cJSON_CreateString("one");
    cJSON *str_two = cJSON_CreateString("two");
    double nums[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag_false = cJSON_CreateFalse();
    (void)version;

    // step 2: Configure
    cJSON_AddItemToArray(values, str_one);
    cJSON_AddItemToArray(values, str_two);
    cJSON_AddItemToArray(values, double_array);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "active", flag_false);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len_snapshot = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len_snapshot + 1);
    memset(buffer, 0, len_snapshot + 1);
    memcpy(buffer, snapshot, len_snapshot + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached_active = cJSON_DetachItemFromObjectCaseSensitive(parsed, "active");
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *third_item = cJSON_GetArrayItem(parsed_values, 2);
    cJSON *first_num_item = cJSON_GetArrayItem(third_item, 0);
    double first_num = cJSON_GetNumberValue(first_num_item);
    (void)first_str;
    (void)first_num;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_active);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}