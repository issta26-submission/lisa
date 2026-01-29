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
//<ID> 1246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"initial\",\"value\":42}";
    cJSON *root = cJSON_Parse(json);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *added_str = cJSON_CreateString("extra-string");
    cJSON_AddItemToObject(root, "added", added_str);

    // step 2: Configure
    char *old_name = cJSON_SetValuestring(name_item, "updated-name");

    // step 3: Operate and Validate
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_bool added_is_string = cJSON_IsString(added_str);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)old_name;
    (void)name_is_string;
    (void)added_is_string;
    return 66;
}