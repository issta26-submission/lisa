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
//<ID> 1222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *name_item = NULL;
    cJSON *arr_item = NULL;
    cJSON *first_elem = NULL;
    cJSON *replacement = NULL;
    const char *parse_end = NULL;
    const char *json = "{\"name\":\"original\",\"arr\":[\"one\",\"two\"]}";
    size_t json_len = sizeof("{\"name\":\"original\",\"arr\":[\"one\",\"two\"]}") - 1;
    int arr_size = 0;
    const char *first_val = NULL;
    const char *new_name_val = NULL;
    cJSON_bool replace_ok = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    arr_item = cJSON_GetObjectItem(parsed, "arr");
    name_item = cJSON_GetObjectItem(parsed, "name");
    replacement = cJSON_CreateString("replaced");
    replace_ok = cJSON_ReplaceItemViaPointer(parsed, name_item, replacement);
    first_elem = cJSON_GetArrayItem(arr_item, 0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr_item);
    first_val = cJSON_GetStringValue(first_elem);
    new_name_val = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "name"));
    (void)arr_size;
    (void)first_val;
    (void)new_name_val;
    (void)replace_ok;
    (void)parse_end;
    (void)json_len;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}