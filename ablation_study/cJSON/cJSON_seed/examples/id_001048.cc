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
//<ID> 1048
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    const char *label_str = "example_label";
    cJSON *label_ref = cJSON_CreateStringReference(label_str);
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 2: Configure
    const char *json1 = "{\"num\":123}";
    const char *endptr1 = NULL;
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(json1, (size_t)11, &endptr1, 1);
    const char *json2 = "[10,20]";
    cJSON *parsed2 = cJSON_Parse(json2);
    cJSON_AddItemToObject(root, "parsed_obj", parsed1);
    cJSON_AddItemToObject(root, "parsed_arr", parsed2);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(parsed1, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    int count = cJSON_GetArraySize(arr);
    double sum_val = num_val + (double)count;
    cJSON *sum = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON_bool is_array = cJSON_IsArray(parsed2);
    cJSON_AddBoolToObject(root, "parsed2_is_array", is_array);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}