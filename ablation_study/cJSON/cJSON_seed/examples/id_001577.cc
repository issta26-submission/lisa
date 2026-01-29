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
//<ID> 1577
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
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON *n2 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "name", "example");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_arr);
    cJSON *child_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(child_item);
    cJSON *computed = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate & Validate
    cJSON *got_copy_item = cJSON_GetArrayItem(dup_arr, 1);
    double second_val = cJSON_GetNumberValue(got_copy_item);
    cJSON *sum = cJSON_CreateNumber(first_val + second_val);
    cJSON_AddItemToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "values_copy");
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}