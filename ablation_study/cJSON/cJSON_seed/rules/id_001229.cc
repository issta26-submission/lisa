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
//<ID> 1229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *item1 = NULL;
    cJSON *new_item = NULL;
    cJSON *first_item = NULL;
    const char *json = "{\"arr\":[\"one\",\"two\",\"three\"]}";
    size_t json_len = sizeof("{\"arr\":[\"one\",\"two\",\"three\"]}") - 1;
    const char *parse_end = NULL;
    int arr_size = 0;
    const char *first_val = NULL;
    const char *replaced_val = NULL;
    cJSON_bool replace_result = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    arr = cJSON_GetObjectItem(parsed, "arr");
    new_item = cJSON_CreateString("replaced");

    // step 3: Operate / Validate
    item1 = cJSON_GetArrayItem(arr, 1);
    replace_result = cJSON_ReplaceItemViaPointer(arr, item1, new_item);
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetStringValue(first_item);
    replaced_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    (void)parse_end;
    (void)replace_result;
    (void)first_val;
    (void)replaced_val;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}