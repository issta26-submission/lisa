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
//<ID> 1251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"root\":{\"name\":\"example\",\"values\":[1.1,2.2,3.3]}}";
    size_t json_len = sizeof(json) - 1;
    const char raw_json[] = "{\"flag\":true}";
    double base_numbers[4] = {4.4, 5.5, 6.6, 7.7};
    double computed_nums[2];
    cJSON *root = NULL;
    cJSON *root_obj = NULL;
    cJSON *name_item = NULL;
    cJSON *values = NULL;
    cJSON *val_item = NULL;
    cJSON *new_arr = NULL;
    cJSON *computed_arr = NULL;
    char *printed = NULL;
    char *name_str = NULL;
    double num = 0.0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    root_obj = cJSON_GetObjectItem(root, "root");
    name_item = cJSON_GetObjectItem(root_obj, "name");
    values = cJSON_GetObjectItem(root_obj, "values");

    // step 3: Operate / Validate
    val_item = cJSON_GetArrayItem(values, 1);
    num = cJSON_GetNumberValue(val_item);
    computed_nums[0] = num;
    computed_nums[1] = num * 2.0;
    new_arr = cJSON_CreateDoubleArray(base_numbers, 4);
    computed_arr = cJSON_CreateDoubleArray(computed_nums, 2);
    cJSON_AddItemToObject(root_obj, "more_values", new_arr);
    cJSON_AddItemToObject(root_obj, "computed", computed_arr);
    cJSON_AddStringToObject(root_obj, "status", "ok");
    cJSON_AddRawToObject(root_obj, "raw", raw_json);
    cJSON_AddNullToObject(root_obj, "missing");
    name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root_obj, "name_copy", name_str);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}