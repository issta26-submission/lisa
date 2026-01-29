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
//<ID> 1043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"values\":[1,2]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *parsed_simple = cJSON_Parse("{\"x\":10}");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON *n2 = cJSON_CreateNumber(200.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *strref = cJSON_CreateStringReference("ref_str");

    // step 2: Configure
    cJSON_AddItemToObject(root, "added_array", arr);
    cJSON_AddItemToObject(root, "ref", strref);
    cJSON_AddItemToObject(parsed_simple, "more", cJSON_CreateNumber(3.14));
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON *name_ref = cJSON_CreateStringReference(name_str);
    cJSON_AddItemToObject(root, "orig_name_ref", name_ref);

    // step 3: Operate & Validate
    double first_arr_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double parsed_x = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed_simple, "x"));
    cJSON *sum_num = cJSON_CreateNumber(first_arr_val + parsed_x);
    cJSON_AddItemToObject(root, "sum", sum_num);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddItemToObject(root, "is_object_flag", cJSON_CreateNumber((double)root_is_obj));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(parsed_simple);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}