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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double values_init[3] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(values_init, 3);

    // step 2: Configure
    cJSON *extra_value = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(values_array, extra_value);
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    double enabled_numeric = (double)cJSON_IsTrue(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_numeric", enabled_numeric);
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    double values_count = (double)cJSON_GetArraySize(values_item);
    cJSON_AddNumberToObject(root, "values_count", values_count);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}