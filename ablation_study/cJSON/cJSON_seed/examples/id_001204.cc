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
//<ID> 1204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":123,\"obj\":{\"x\":10,\"y\":20}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddNumberToObject(root, "neg", -42.0);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddNumberToObject(inner, "inner_val", 99.0);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 2: Configure
    cJSON *moved = cJSON_DetachItemFromObject(parsed, "obj");
    cJSON_AddItemToObject(root, "moved_obj", moved);

    // step 3: Operate & Validate
    cJSON_bool is_invalid = cJSON_IsInvalid(moved);
    cJSON *invalid_flag = cJSON_CreateBool(is_invalid);
    cJSON_AddItemToObject(root, "moved_invalid", invalid_flag);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}