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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *num1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(config, "num1", num1);
    cJSON *num2 = cJSON_AddNumberToObject(config, "num2", 7.25);

    // step 2: Configure
    cJSON_SetNumberHelper(num1, 5.5);
    cJSON *num2_ptr = cJSON_GetObjectItem(config, "num2");
    cJSON_SetNumberHelper(num2_ptr, 8.75);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 3: Operate and Validate
    double v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "num1"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "num2"));
    cJSON *sum_item = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    double dup_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root_dup, "sum"));
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}