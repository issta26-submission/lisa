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
//<ID> 2366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *intArray = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", intArray);
    cJSON *manualNum = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "manual_number", manualNum);

    // step 2: Configure
    cJSON *autoNum = cJSON_AddNumberToObject(root, "auto_number", 7.5);
    cJSON *flag = cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *ints = cJSON_GetObjectItem(root, "ints");
    cJSON *second = cJSON_GetArrayItem(ints, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *manual = cJSON_GetObjectItem(root, "manual_number");
    double manual_val = cJSON_GetNumberValue(manual);
    buffer[0] = (char)(int)second_val;
    buffer[1] = (char)(int)manual_val;
    buffer[2] = (printed && printed[0]) ? printed[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}