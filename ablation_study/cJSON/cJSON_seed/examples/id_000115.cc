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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    size_t buf_size = 64;
    char *raw_json = (char *)cJSON_malloc(buf_size);
    memset(raw_json, 0, buf_size);
    strcpy(raw_json, "  [ null , 123 , \"text\" ]  ");
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToArray(items, raw_item);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(parsed, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *flag = cJSON_CreateNumber((double)first_is_null);
    cJSON_AddItemToObject(root, "first_was_null", flag);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(raw_json);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}