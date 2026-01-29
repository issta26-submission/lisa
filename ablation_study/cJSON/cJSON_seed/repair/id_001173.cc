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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddNullToObject(user, "optional");

    // step 2: Configure
    cJSON *score = cJSON_AddNumberToObject(user, "score", 42.5);
    cJSON_AddBoolToObject(user, "verified", 1);
    cJSON *name_ref = cJSON_GetObjectItem(user, "name");
    char *setret = cJSON_SetValuestring(name_ref, "updated");
    (void)setret;

    // step 3: Operate and Validate
    cJSON_bool opt_is_null = cJSON_IsNull(cJSON_GetObjectItem(user, "optional"));
    double base_score = cJSON_GetNumberValue(score);
    double final_score = base_score + 10.0 + (double)opt_is_null;
    cJSON_AddNumberToObject(root, "final_score", final_score);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}