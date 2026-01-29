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
//<ID> 543
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
    int values[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(values, 3);
    cJSON_AddItemToObject(root, "numbers", int_array);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.5);

    // step 3: Operate & Validate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *detached_scale = cJSON_DetachItemFromObjectCaseSensitive(root, "scale");
    double scale_value = cJSON_GetNumberValue(detached_scale);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;first=%.2f;scale=%.2f", version ? version : "null", first_value, scale_value);

    // step 4: Cleanup
    cJSON_Delete(detached_scale);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}