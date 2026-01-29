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
    const char *json_text = "{\"alpha\": 1, \"beta\": 2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *child = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "greeting", greet);
    cJSON_AddNullToObject(root, "explicit_null");
    cJSON_bool is_arr = cJSON_IsArray(numbers);
    cJSON_AddBoolToObject(root, "numbers_is_array", is_arr);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, n2);
    cJSON_AddItemToObject(root, "references", refs);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(numbers, first_item, replacement);
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}