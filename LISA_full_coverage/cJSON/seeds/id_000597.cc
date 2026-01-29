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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"values\": [1.1, 2.2, 3.3], \"meta\": {\"scale\": 2.0}}";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double second_before = cJSON_GetNumberValue(second_item);
    const double new_nums[4] = {4.4, 5.5, 6.6, 7.7};
    cJSON *new_arr = cJSON_CreateDoubleArray(new_nums, 4);
    cJSON_bool add_ok = cJSON_AddItemToObject(root, "new_values", new_arr);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(second_before * 10.0);
    cJSON_bool replace_ok = cJSON_ReplaceItemViaPointer(values, second_item, replacement);
    cJSON *second_after_item = cJSON_GetArrayItem(values, 1);
    double second_after = cJSON_GetNumberValue(second_after_item);

    // step 4: Validate & Cleanup
    (void)add_ok;
    (void)replace_ok;
    (void)second_before;
    (void)second_after;
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}