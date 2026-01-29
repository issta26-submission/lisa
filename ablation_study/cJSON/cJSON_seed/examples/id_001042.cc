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
//<ID> 1042
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"base\": 1.5, \"list\": [2, 3]}";
    const char json2[] = "{\"other\": 7}";
    const char *endptr = json2;
    cJSON *root1 = cJSON_Parse(json1);
    cJSON *root2 = cJSON_ParseWithLengthOpts(json2, (size_t)(sizeof(json2) - 1), &endptr, 1);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *n42 = cJSON_CreateNumber(42.0);
    cJSON *sref = cJSON_CreateStringReference("hello-ref");
    cJSON_AddItemToArray(arr, n42);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root1, "appended", arr);
    cJSON *endref = cJSON_CreateStringReference(endptr);
    cJSON_AddItemToObject(root1, "parse_end", endref);

    // step 3: Operate & Validate
    cJSON *base_item = cJSON_GetObjectItem(root1, "base");
    double base_val = cJSON_GetNumberValue(base_item);
    cJSON *other_item = cJSON_GetObjectItem(root2, "other");
    double other_val = cJSON_GetNumberValue(other_item);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_in_arr = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_in_arr);
    cJSON *computed = cJSON_CreateNumber(base_val + other_val + first_val);
    cJSON_AddItemToObject(root1, "computed_sum", computed);
    char *out = cJSON_PrintUnformatted(root1);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root2);
    cJSON_Delete(root1);
    // API sequence test completed successfully
    return 66;
}