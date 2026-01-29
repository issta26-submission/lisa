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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"user\":{\"name\":\"Bob\",\"age\":25},\"status\":\"ok\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *user = cJSON_GetObjectItem(root, "user");

    // step 2: Configure
    cJSON *new_user = cJSON_CreateObject();
    cJSON_AddStringToObject(new_user, "name", "Robert");
    cJSON_AddNumberToObject(new_user, "age", 26.0);
    cJSON_AddTrueToObject(new_user, "active");
    cJSON_ReplaceItemViaPointer(root, user, new_user);

    // step 3: Operate and Validate
    cJSON *updated_user = cJSON_GetObjectItem(root, "user");
    cJSON *name_item = cJSON_GetObjectItem(updated_user, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *age_item = cJSON_GetObjectItem(updated_user, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    double computed_val = age_val + 10.0;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "age_plus", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON *detached = cJSON_CreateString("temp");
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}