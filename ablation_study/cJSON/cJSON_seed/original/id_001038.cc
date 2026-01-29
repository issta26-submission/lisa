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
//<ID> 1038
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"num\":42,\"str\":\"initial\"}";
    size_t json_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *raw_item = cJSON_CreateRaw("/*raw-content*/");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *str_item = cJSON_GetObjectItem(root, "str");
    char *new_vs = cJSON_SetValuestring(str_item, "modified");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "source_copy", cJSON_Duplicate(root, 1));
    cJSON_AddItemToObject(out, "was_raw", cJSON_CreateBool(raw_flag));
    cJSON_AddItemToObject(out, "modified_str_ref", cJSON_CreateString(new_vs));

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}