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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":1,\"b\":2,\"nested\":{\"x\":10}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *added_pi = cJSON_AddNumberToObject(root, "added_pi", 3.1415);
    cJSON *detached_nested = cJSON_DetachItemFromObject(parsed, "nested");
    cJSON_bool add_detached_ok = cJSON_AddItemToObject(root, "detached", detached_nested);

    // step 3: Operate & Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "added_pi");
    cJSON_bool invalid_flag = cJSON_IsInvalid(got_pi);
    int status = add_detached_ok + (1 - invalid_flag);
    cJSON *status_item = cJSON_AddNumberToObject(root, "status", (double)status);
    char *out_root = cJSON_Print(root);
    char *out_parsed = cJSON_Print(parsed);
    cJSON_free(out_root);
    cJSON_free(out_parsed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}