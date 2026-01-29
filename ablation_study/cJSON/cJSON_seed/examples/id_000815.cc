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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items[] = { "one", "two", "three" };
    cJSON *arr = cJSON_CreateStringArray(items, 3);
    cJSON *flag_false = cJSON_CreateBool((cJSON_bool)0);
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_AddItemToObject(root, "always_true", true_item);
    cJSON_AddNullToObject(root, "missing");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *second_item = cJSON_GetArrayItem(parsed_list, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    cJSON *copied = cJSON_CreateString(second_value);
    cJSON_AddItemToObject(parsed, "copied_second", copied);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}