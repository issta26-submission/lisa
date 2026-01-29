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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n3);
    cJSON *label_ref = cJSON_CreateStringReference("num-set");
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    const char *inline_arr = "[4,5,6]";
    const char *parse_end = NULL;
    cJSON *parsed_array = cJSON_ParseWithLengthOpts(inline_arr, (size_t)7, &parse_end, 1);
    const char *obj_json = "{\"x\": 10.5}";
    cJSON *parsed_obj = cJSON_Parse(obj_json);
    cJSON *x_item = cJSON_GetObjectItem(parsed_obj, "x");
    double x_val = cJSON_GetNumberValue(x_item);

    // step 3: Operate & Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    double total = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + x_val;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    cJSON_AddItemToObject(root, "parsed_values", parsed_array);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed_obj);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}