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
//<ID> 1416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *version_ref = cJSON_CreateStringReference(version);
    double numbers[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddItemToObject(root, "values", double_array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(double_array));

    // step 3: Operate & Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    int values_size = cJSON_GetArraySize(values_item);
    cJSON *first_value_item = cJSON_GetArrayItem(values_item, 0);
    double first_value = cJSON_GetNumberValue(first_value_item);
    (void)count_value;
    (void)values_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}