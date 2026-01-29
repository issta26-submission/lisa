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
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *obj_ref = NULL;
    cJSON *names_arr = NULL;
    cJSON *bob_item = NULL;
    cJSON *new_bob = NULL;
    cJSON *meta = NULL;
    cJSON *greet_item = NULL;
    cJSON *new_greet = NULL;
    cJSON *first_item = NULL;
    const char *json = "{\"names\":[\"alice\",\"bob\",\"carol\"],\"meta\":{\"greeting\":\"hello\"}}";
    size_t json_len = sizeof("{\"names\":[\"alice\",\"bob\",\"carol\"],\"meta\":{\"greeting\":\"hello\"}}") - 1;
    const char *parse_end = NULL;
    char *printed = NULL;
    const char *first_name = NULL;
    const char *greeting = NULL;
    int arr_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    obj_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObject(root, "parsed_ref", obj_ref);
    names_arr = cJSON_GetObjectItem(parsed, "names");
    bob_item = cJSON_GetArrayItem(names_arr, 1);
    new_bob = cJSON_CreateString("zed");
    replaced = cJSON_ReplaceItemViaPointer(names_arr, bob_item, new_bob);
    meta = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(meta, "greeting");
    new_greet = cJSON_CreateString("hi");
    replaced = cJSON_ReplaceItemViaPointer(meta, greet_item, new_greet);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(names_arr);
    first_item = cJSON_GetArrayItem(names_arr, 0);
    first_name = cJSON_GetStringValue(first_item);
    greeting = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "greeting"));
    printed = cJSON_PrintUnformatted(parsed);
    (void)arr_size;
    (void)first_name;
    (void)greeting;
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}