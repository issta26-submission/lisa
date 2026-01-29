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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"keep\":42,\"remove\":7}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "remove");
    double removed_val = cJSON_GetNumberValue(detached);
    cJSON *added_number = cJSON_AddNumberToObject(root, "reused_value", removed_val);

    // step 3: Operate
    cJSON_bool detached_is_invalid = cJSON_IsInvalid(detached);
    cJSON *flag = cJSON_AddBoolToObject(root, "detached_invalid", detached_is_invalid);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}