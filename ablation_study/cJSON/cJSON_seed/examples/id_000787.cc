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
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {4.5, 5.5, 6.5};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *id_item = cJSON_CreateNumber(7.0);
    cJSON *name_item = cJSON_CreateString("test-label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "id", id_item);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_dup = cJSON_PrintUnformatted(root_dup);
    cJSON_bool are_equal = cJSON_Compare(root, root_dup, 1);
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(retrieved_values);
    cJSON *first_value_item = cJSON_GetArrayItem(retrieved_values, 0);
    double first_value = cJSON_GetNumberValue(first_value_item);
    cJSON *retrieved_id = cJSON_GetObjectItem(root, "id");
    double id_value = cJSON_GetNumberValue(retrieved_id);
    double aggregate = id_value + first_value + (double)values_count;
    (void)aggregate;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_dup);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}