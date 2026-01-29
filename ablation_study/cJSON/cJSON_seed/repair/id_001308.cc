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
//<ID> 1308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON *note = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "note", note);

    // step 2: Configure
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(retrieved_flag);
    cJSON_bool flag_is_true = cJSON_IsTrue(retrieved_flag);
    const char *states[2] = { "disabled", "enabled" };
    int state_index = (int)flag_is_true;
    cJSON_AddStringToObject(root, "state", states[state_index]);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)flag_is_bool;
    ((char *)scratch)[2] = (char)flag_is_true;
    char *state_val = (char *)cJSON_GetStringValue(cJSON_GetObjectItem(root, "state"));
    ((char *)scratch)[3] = state_val ? state_val[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}