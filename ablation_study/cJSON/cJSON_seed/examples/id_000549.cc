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
//<ID> 549
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
    int nums[3];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *orig_pi = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 3: Operate & Validate
    double orig_pi_val = cJSON_GetNumberValue(orig_pi);
    cJSON *detached_pi = cJSON_DetachItemFromObjectCaseSensitive(root, "pi");
    double detached_pi_val = cJSON_GetNumberValue(detached_pi);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double parsed_pi_val = cJSON_GetNumberValue(parsed_pi);

    // step 4: Cleanup
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;orig=%.5f;detached=%.5f;parsed=%.5f",
            version ? version : "null",
            orig_pi_val,
            detached_pi_val,
            parsed_pi_val);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(detached_pi);
    // API sequence test completed successfully
    return 66;
}