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
//<ID> 1228
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
    cJSON *c1 = NULL;
    cJSON *c2 = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *orig_first = NULL;
    cJSON *replacement = NULL;
    const char *json = "{\"names\":[\"Alice\",\"Bob\"]}";
    size_t json_len = sizeof("{\"names\":[\"Alice\",\"Bob\"]}") - 1;
    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 1;
    int parsed_size = 0;
    const char *first_val = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, require_null_terminated);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    c1 = cJSON_CreateString("Alice_new");
    c2 = cJSON_CreateString("Bob_new");
    cJSON_AddItemToArray(arr, c1);
    cJSON_AddItemToArray(arr, c2);
    cJSON_AddItemToObject(root, "names", arr);

    // step 3: Operate / Validate
    parsed_arr = cJSON_GetObjectItem(parsed, "names");
    orig_first = cJSON_GetArrayItem(parsed_arr, 0);
    replacement = cJSON_Duplicate(c1, 1);
    cJSON_ReplaceItemViaPointer(parsed_arr, orig_first, replacement);
    parsed_size = cJSON_GetArraySize(parsed_arr);
    first_val = cJSON_GetStringValue(cJSON_GetArrayItem(parsed_arr, 0));
    (void)parsed_size;
    (void)first_val;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}