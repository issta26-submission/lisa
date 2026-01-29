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
//<ID> 1209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":42,\"b\":99}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "a");
    cJSON_AddItemToObject(root, "migrated_a", detached);
    cJSON_AddNumberToObject(root, "x", 3.1415);

    // step 3: Operate & Validate
    cJSON_bool invalid_flag = cJSON_IsInvalid(detached);
    cJSON_AddNumberToObject(root, "detached_invalid", (double)invalid_flag);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}