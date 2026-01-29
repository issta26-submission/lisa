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
//<ID> 1388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[\"one\",\"two\",\"three\"],\"flag\":false}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("TWO");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *copied = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "copied_first", copied);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}