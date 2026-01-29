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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text_a = "{\"status\":\"ok\",\"active\":true,\"count\":3}";
    const char *json_text_b = "{\"status\":\"ok\",\"active\":false,\"count\":3}";
    cJSON *parsed_a = cJSON_Parse(json_text_a);
    cJSON *parsed_b = cJSON_Parse(json_text_b);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *label = cJSON_CreateString("session_id");
    cJSON *online_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "online", online_flag);
    cJSON *orig_dup = cJSON_Duplicate(parsed_a, 1);
    cJSON_AddItemToObject(root, "orig", orig_dup);

    // step 3: Operate & Validate
    cJSON_bool equal_strict = cJSON_Compare(parsed_a, parsed_b, 1);
    cJSON *cmp_flag = cJSON_CreateBool(equal_strict);
    cJSON_AddItemToObject(root, "strict_equal", cmp_flag);
    cJSON *note = cJSON_CreateString("compared");
    cJSON_AddItemToObject(root, "note", note);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed_a);
    cJSON_Delete(parsed_b);
    // API sequence test completed successfully
    return 66;
}