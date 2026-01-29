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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[false,false,false],\"template\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *item0 = cJSON_CreateFalse();
    cJSON *item1 = cJSON_CreateFalse();
    cJSON *item2 = cJSON_CreateFalse();
    cJSON_AddItemToArray(created_arr, item0);
    cJSON_AddItemToArray(created_arr, item1);
    cJSON_AddItemToArray(created_arr, item2);
    cJSON_AddItemToObject(root, "created", created_arr);

    // step 3: Operate & Validate
    cJSON *template_item = cJSON_GetObjectItem(parsed, "template");
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInArray(created_arr, 0, new_true);
    cJSON_bool equal = cJSON_Compare(template_item, new_true, 1);
    cJSON *cmp_bool = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "template_matches", cmp_bool);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}