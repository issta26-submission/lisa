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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 123.456);
    cJSON_AddItemToArray(arr, num);
    cJSON *elem_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(elem_obj, "name", "sample");
    cJSON_AddItemToArray(arr, elem_obj);

    // step 2: Configure
    char to_minify[] = "  { \"k1\" : 10 , \"k2\" : 20 }  ";
    cJSON_Minify(to_minify);
    cJSON *parsed = cJSON_ParseWithLength(to_minify, (size_t)(sizeof(to_minify) - 1));
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed", dup_parsed);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));
    cJSON_AddStringToObject(root, "replace_me", "old_value");

    // step 3: Operate
    cJSON *new_replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_replacement, "replaced_at", 1.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", new_replacement);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}