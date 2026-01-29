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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *parsed_first = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    const char parsed_json[] = "{\"parsed\": [\"alpha\",\"beta\"]}";
    size_t parsed_len = sizeof(parsed_json) - 1;
    const char *parse_end = NULL;
    int parsed_size = 0;
    int arr_size = 0;
    const char *first_val = NULL;
    const char *new_first = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(parsed_json, parsed_len, &parse_end, 1);
    parsed_arr = cJSON_GetObjectItem(parsed, "parsed");
    parsed_size = cJSON_GetArraySize(parsed_arr);
    parsed_first = cJSON_GetArrayItem(parsed_arr, 0);
    first_val = cJSON_GetStringValue(parsed_first);

    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "letters", arr);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("placeholder");
    cJSON_SetValuestring(replacement, first_val);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_ReplaceItemViaPointer(arr, item0, replacement);
    printed = cJSON_PrintBuffered(root, arr_size + 50, 0);
    new_first = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)new_first;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}