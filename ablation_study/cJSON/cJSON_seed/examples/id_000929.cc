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
//<ID> 929
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
    cJSON *device_str = cJSON_CreateString("device-42");
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON *status_item = cJSON_CreateString("old");

    // step 2: Configure
    cJSON_AddItemToArray(arr, device_str);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "status", status_item);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetArrayItem(arr, 1);
    double updated = cJSON_SetNumberHelper(got_num, 42.5);
    cJSON *replacement_status = cJSON_CreateString("new-status");
    cJSON_ReplaceItemInObject(root, "status", replacement_status);
    cJSON *retrieved_status = cJSON_GetObjectItem(root, "status");
    char *status_text = cJSON_GetStringValue(retrieved_status);
    (void)status_text;
    (void)updated;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}