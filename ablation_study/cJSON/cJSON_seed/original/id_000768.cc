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
//<ID> 768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *inner_flag = cJSON_AddBoolToObject(child, "inner_flag", 1);
    cJSON_bool add_ret1 = cJSON_AddItemToObjectCS(root, "child", child);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool add_ret2 = cJSON_AddItemToObjectCS(root, "always_true", true_item);
    cJSON *child_ref = cJSON_Duplicate(child, 1);
    cJSON_bool add_ret3 = cJSON_AddItemToObjectCS(root, "child_ref", child_ref);

    // step 2: Configure
    char json_buf[] = "  { \"parsed\": true, \"value\": false }  ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *parsed_bool = cJSON_GetObjectItem(parsed, "parsed");
    cJSON *dup_parsed_bool = cJSON_Duplicate(parsed_bool, 0);
    cJSON_bool add_ret4 = cJSON_AddItemToObjectCS(root, "parsed_flag", dup_parsed_bool);

    // step 3: Operate
    cJSON *direct_bool = cJSON_AddBoolToObject(root, "direct_flag", 0);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
}