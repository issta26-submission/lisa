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
//<ID> 1555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *values_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddNumberToObject(root, "scale", 2.5);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON *duplicate_root = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON *orig_values = cJSON_GetObjectItem(root, "values");
    int orig_size = cJSON_GetArraySize(orig_values);
    cJSON *orig_first = cJSON_GetArrayItem(orig_values, 0);
    double orig_first_val = cJSON_GetNumberValue(orig_first);
    cJSON *dup_values = cJSON_GetObjectItem(duplicate_root, "values");
    int dup_size = cJSON_GetArraySize(dup_values);
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    double dup_first_val = cJSON_GetNumberValue(dup_first);
    (void)orig_size; (void)orig_first_val; (void)dup_size; (void)dup_first_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}