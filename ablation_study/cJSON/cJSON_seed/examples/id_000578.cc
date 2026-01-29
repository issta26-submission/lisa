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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateArray();
    const char *items[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(items, 3);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToArray(container, str_array);
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddStringToObject(root, "title", "example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool is_false = cJSON_IsFalse(parsed_flag);
    cJSON *parsed_container = cJSON_GetObjectItem(parsed, "container");
    cJSON *first_element = cJSON_GetArrayItem(parsed_container, 0);
    char *first_str = cJSON_GetStringValue(first_element);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;is_false=%d;first=%s",
            version ? version : "null",
            is_false ? 1 : 0,
            first_str ? first_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}