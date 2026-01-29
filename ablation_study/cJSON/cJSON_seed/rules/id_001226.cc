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
//<ID> 1226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *replacement = NULL;
    cJSON *first_item = NULL;
    const char *json = "[\"one\", \"two\"]";
    size_t json_len = sizeof("[\"one\", \"two\"]") - 1;
    const char *parse_end = NULL;
    int arr_size = 0;
    const char *first_str = NULL;
    cJSON_bool replace_ok = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "list", arr);
    replacement = cJSON_CreateString("uno");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    replace_ok = cJSON_ReplaceItemViaPointer(arr, first_item, replacement);
    first_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)arr_size;
    (void)replace_ok;
    (void)first_str;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}