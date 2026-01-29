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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *values_arr = (cJSON *)0;
    cJSON *new_num1 = (cJSON *)0;
    cJSON *new_num2 = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *nested_obj = (cJSON *)0;
    cJSON *nested_n = (cJSON *)0;
    const char *json_text = "{\"values\": [1.5, 2.5], \"nested\": {\"n\": 3.75}}";
    const char *parse_end = (const char *)0;
    cJSON_bool require_null_terminated = (cJSON_bool)1;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double vn = 0.0;

    // step 2: Initialize (parse JSON)
    root = cJSON_ParseWithOpts(json_text, &parse_end, require_null_terminated);

    // step 3: Configure (retrieve array and append new numbers)
    values_arr = cJSON_GetObjectItem(root, "values");
    new_num1 = cJSON_CreateNumber(4.25);
    new_num2 = cJSON_CreateNumber(5.5);
    add_ok1 = cJSON_AddItemToArray(values_arr, new_num1);
    add_ok2 = cJSON_AddItemToArray(values_arr, new_num2);

    // step 4: Operate (retrieve specific items and nested value)
    arr_item0 = cJSON_GetArrayItem(values_arr, 0);
    arr_item1 = cJSON_GetArrayItem(values_arr, 2); // originally index 2 after adding one item
    v0 = cJSON_GetNumberValue(arr_item0);
    v1 = cJSON_GetNumberValue(arr_item1);
    nested_obj = cJSON_GetObjectItemCaseSensitive(root, "nested");
    nested_n = cJSON_GetObjectItemCaseSensitive(nested_obj, "n");
    vn = cJSON_GetNumberValue(nested_n);

    // step 5: Validate (use variables to avoid unused warnings)
    (void)parse_end;
    (void)require_null_terminated;
    (void)add_ok1;
    (void)add_ok2;
    (void)v0;
    (void)v1;
    (void)vn;

    // step 6: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}