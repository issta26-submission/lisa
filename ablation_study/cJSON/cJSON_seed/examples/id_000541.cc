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
//<ID> 541
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

    // step 2: Configure
    int numbers[3];
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    cJSON *ints = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *val_item = cJSON_AddNumberToObject(nested, "val", 42.0);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "nested");
    cJSON *detached_val = cJSON_GetObjectItem(detached, "val");
    double extracted = cJSON_GetNumberValue(detached_val);
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;pi=%.5f;extracted=%.1f",
            version ? version : "null",
            cJSON_GetNumberValue(pi_item),
            extracted);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}