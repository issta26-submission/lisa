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
//<ID> 1559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    cJSON *array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON_AddNumberToObject(root, "version", 1.5);
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_array);
    cJSON *shallow_dup = cJSON_Duplicate(root, 0);

    // step 2: Configure
    char *printed = cJSON_Print(shallow_dup);
    int array_size = cJSON_GetArraySize(array);

    // step 3: Operate & Validate
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    double version_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    cJSON *computed = cJSON_CreateNumber(first_value + version_value + (double)array_size);
    cJSON_AddItemToObject(root, "computed_sum", computed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(shallow_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}