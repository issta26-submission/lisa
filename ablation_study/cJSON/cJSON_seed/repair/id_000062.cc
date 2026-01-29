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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *profile = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "profile", profile);
    cJSON *name = cJSON_CreateString("user");
    cJSON_AddItemToObject(profile, "name", name);
    cJSON *age = cJSON_AddNumberToObject(profile, "age", 30.0);

    // step 2: Configure
    cJSON_SetValuestring(name, "tester");
    cJSON_bool has_age = cJSON_HasObjectItem(profile, "age");

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name);
    cJSON_AddNumberToObject(root, "name_code", (double)name_val[0]);
    cJSON_AddNumberToObject(root, "has_age", (double)has_age);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}