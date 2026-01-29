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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":1,\"b\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "x", 42.0);
    cJSON_AddNumberToObject(root, "y", 3.14);
    cJSON *detached_b = cJSON_DetachItemFromObject(parsed, "b");
    cJSON_AddItemToObject(root, "moved_b", detached_b);

    // step 3: Operate & Validate
    double invalid_flag = (double)cJSON_IsInvalid(detached_b);
    cJSON_AddNumberToObject(root, "detached_is_invalid", invalid_flag);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}