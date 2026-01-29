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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"orig\",\"nums\":[1,2,3]}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *new_name = cJSON_SetValuestring(name_item, "updated_name");
    (void)new_name;
    cJSON *raw_child = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "raw_added", raw_child);

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_child);
    (void)raw_flag;
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}