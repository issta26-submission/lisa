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
//<ID> 1550
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed_dup = cJSON_Print(dup_root);

    // step 3: Operate & Validate
    int original_size = cJSON_GetArraySize(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *dup_values = cJSON_GetObjectItem(dup_root, "values");
    int dup_size = cJSON_GetArraySize(dup_values);
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    double dup_first_value = cJSON_GetNumberValue(dup_first);
    double combined = first_value + dup_first_value;
    cJSON *combined_number = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "sum", combined_number);
    (void)original_size; (void)dup_size; (void)printed_root; (void)printed_dup;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}