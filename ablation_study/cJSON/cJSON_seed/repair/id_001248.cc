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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"orig\",\"count\":2,\"list\":[\"a\",\"b\"]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *name = cJSON_GetObjectItem(root, "name");
    cJSON *list = cJSON_GetObjectItem(root, "list");
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToObject(root, "extra", extra);

    // step 2: Configure
    cJSON_bool name_was_string = cJSON_IsString(name);
    char *set_result = cJSON_SetValuestring(name, "modified");
    cJSON_AddItemToArray(list, cJSON_CreateString("c"));

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(list, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = (set_result && set_result[0]) ? set_result[0] : 0;
    ((char *)scratch)[3] = (char)(name_was_string + first_is_string);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}