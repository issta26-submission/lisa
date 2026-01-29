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
//<ID> 1216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[1.5,2.5,3.5],\"remove_me\":123}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON *arr_ref = cJSON_CreateArrayReference(created_arr);
    cJSON_AddItemToObject(root, "referenced_array", arr_ref);

    // step 3: Operate & Validate
    cJSON *parsed_vals = cJSON_GetObjectItem(parsed, "values");
    cJSON *first_val = cJSON_GetArrayItem(parsed_vals, 0);
    double extracted = cJSON_GetNumberValue(first_val);
    cJSON_AddNumberToObject(root, "extracted_first_value", extracted);
    cJSON_DeleteItemFromObject(parsed, "remove_me");
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(created_arr);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}