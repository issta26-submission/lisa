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
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\":[\"one\",\"two\"],\"replace\":\"old\"}";
    size_t json_len = sizeof("{\"arr\":[\"one\",\"two\"],\"replace\":\"old\"}") - 1;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *first_item = NULL;
    cJSON *replace_item = NULL;
    cJSON *new_item = NULL;
    char *printed = NULL;
    const char *first_val = NULL;
    int arr_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 0);
    arr = cJSON_GetObjectItem(parsed, "arr");
    first_item = cJSON_GetArrayItem(arr, 0);
    replace_item = cJSON_GetObjectItem(parsed, "replace");
    new_item = cJSON_CreateString("new");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetStringValue(first_item);
    replaced = cJSON_ReplaceItemViaPointer(parsed, replace_item, new_item);
    printed = cJSON_PrintUnformatted(parsed);
    (void)first_val;
    (void)arr_size;
    (void)replaced;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}