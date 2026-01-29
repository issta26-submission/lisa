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
//<ID> 1227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"letters\": [\"alpha\", \"beta\"], \"meta\": {\"note\": \"ok\"}}";
    size_t json_len = sizeof("{\"letters\": [\"alpha\", \"beta\"], \"meta\": {\"note\": \"ok\"}}") - 1;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *parsed_letters = NULL;
    cJSON *parsed_first = NULL;
    cJSON *arr_first = NULL;
    cJSON *replacement = NULL;
    char *first_val = NULL;
    char *new_first_val = NULL;
    int parsed_size = 0;
    int arr_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("orig1");
    s2 = cJSON_CreateString("orig2");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);

    // step 3: Operate / Validate
    parsed_letters = cJSON_GetObjectItem(parsed, "letters");
    parsed_size = cJSON_GetArraySize(parsed_letters);
    parsed_first = cJSON_GetArrayItem(parsed_letters, 0);
    first_val = cJSON_GetStringValue(parsed_first);
    replacement = cJSON_CreateString(first_val);
    arr_first = cJSON_GetArrayItem(arr, 0);
    replaced = cJSON_ReplaceItemViaPointer(arr, arr_first, replacement);
    new_first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)parsed_size;
    (void)arr_size;
    (void)new_first_val;
    (void)replaced;
    (void)first_val;
    // API sequence test completed successfully
    return 66;
}