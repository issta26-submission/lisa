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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":1,\"beta\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *added = cJSON_AddNumberToObject(root, "added_one", 42.5);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "alpha");
    cJSON_bool invalid_flag = cJSON_IsInvalid(detached);
    cJSON *marker = cJSON_AddNumberToObject(root, "detached_invalid", (double)invalid_flag);
    cJSON_AddItemToObject(root, "moved_alpha", detached);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}