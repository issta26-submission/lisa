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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"x\":1,\"y\":[10,20]}";
    size_t len1 = sizeof(json1) - 1;
    const char *end1 = NULL;
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(json1, len1, &end1, 1);
    const char json2[] = "[\"alpha\",\"beta\"]";
    cJSON *parsed2 = cJSON_Parse(json2);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON *sref = cJSON_CreateStringReference("label_ref");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "extras", arr);
    cJSON_AddItemToObject(root, "parsed", parsed1);
    cJSON_AddItemToObject(root, "names", parsed2);
    cJSON_AddItemToObject(root, "ref", sref);

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *arr_item = cJSON_GetArrayItem(arr, 1);
    double arr_val = cJSON_GetNumberValue(arr_item);
    cJSON *parsed_obj = cJSON_GetObjectItem(root, "parsed");
    cJSON *x_item = cJSON_GetObjectItem(parsed_obj, "x");
    double x_val = cJSON_GetNumberValue(x_item);
    double sum = arr_val + x_val;
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}