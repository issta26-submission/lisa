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
//<ID> 1556
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure & Duplicate
    cJSON *duplicate = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    char *printed_root = cJSON_Print(root);
    char *printed_dup = cJSON_Print(duplicate);
    int size_values = cJSON_GetArraySize(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *dup_values = cJSON_GetObjectItem(duplicate, "values");
    int size_dup_values = cJSON_GetArraySize(dup_values);
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    double dup_first_value = cJSON_GetNumberValue(dup_first);
    double sum = first_value + dup_first_value + (double)size_values + (double)size_dup_values;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}