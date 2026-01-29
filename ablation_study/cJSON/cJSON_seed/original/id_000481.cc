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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"Alice\",\"age\":30}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_was_string = cJSON_IsString(name_item);

    // step 2: Configure
    cJSON *presence_flag = cJSON_CreateBool(name_was_string);
    cJSON_AddItemToObject(root, "name_present", presence_flag);
    char *new_name = (char *)cJSON_malloc(4);
    new_name[0] = 'B';
    new_name[1] = 'o';
    new_name[2] = 'b';
    new_name[3] = '\0';
    cJSON *new_name_ref = cJSON_CreateStringReference((const char *)new_name);
    cJSON_ReplaceItemInObject(root, "name", new_name_ref);

    // step 3: Operate
    cJSON *repl_name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool repl_is_string = cJSON_IsString(repl_name_item);
    cJSON *after_flag = cJSON_CreateBool(repl_is_string);
    cJSON_AddItemToObject(root, "name_is_string_after", after_flag);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_free(new_name);
    // API sequence test completed successfully
    return 66;
}