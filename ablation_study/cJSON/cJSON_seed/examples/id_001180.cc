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
//<ID> 1180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,false,null],\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n10 = cJSON_CreateNumber(10.0);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(new_arr, n10);
    cJSON_AddItemToArray(new_arr, bfalse);
    cJSON_AddItemToObject(root, "new_arr", new_arr);

    // step 2: Configure
    cJSON *orig_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *item1 = cJSON_GetArrayItem(orig_arr, 1);
    cJSON *replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(orig_arr, item1, replacement);

    // step 3: Operate & Validate
    cJSON *checked_item0 = cJSON_GetArrayItem(orig_arr, 0);
    double first_num = cJSON_GetNumberValue(checked_item0);
    cJSON_AddNumberToObject(root, "first_num_copy", first_num);
    cJSON_AddNullToObject(root, "copied_null");
    cJSON *replaced_item = cJSON_GetArrayItem(orig_arr, 1);
    cJSON_AddBoolToObject(root, "replaced_is_true", cJSON_IsTrue(replaced_item));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}