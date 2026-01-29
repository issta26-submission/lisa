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
//<ID> 1220
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
    cJSON *obj_ref = NULL;
    cJSON *array = NULL;
    cJSON *first_item = NULL;
    cJSON *replace_item = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    char *first_val = NULL;
    char *replaced_val = NULL;
    int arr_size = 0;
    cJSON_bool replace_result = 0;
    const char *json = "{\"items\":[\"one\",\"two\"],\"replace_me\":\"old\"}";
    size_t json_len = sizeof("{\"items\":[\"one\",\"two\"],\"replace_me\":\"old\"}") - 1;
    const char *parse_end = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    obj_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObject(root, "parsed_ref", obj_ref);
    replace_item = cJSON_GetObjectItem(parsed, "replace_me");
    replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemViaPointer(parsed, replace_item, replacement);

    // step 3: Operate / Validate
    array = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(array);
    first_item = cJSON_GetArrayItem(array, 0);
    first_val = cJSON_GetStringValue(first_item);
    replaced_val = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "replace_me"));
    printed = cJSON_PrintUnformatted(root);
    (void)first_val;
    (void)replaced_val;
    (void)arr_size;
    (void)replace_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}