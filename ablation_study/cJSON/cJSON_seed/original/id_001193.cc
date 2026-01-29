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
//<ID> 1193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = " [ \"keep\" , \"remove\" , \"keep2\" ] ";
    size_t raw_len = sizeof(raw_json);
    char *buffer = (char *)cJSON_malloc(raw_len);
    memset(buffer, 0, raw_len);
    memcpy(buffer, raw_json, raw_len);
    cJSON_Minify(buffer);
    cJSON *parsed_array = cJSON_Parse(buffer);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "items", parsed_array);
    cJSON *extra_str = cJSON_CreateString("extra");
    cJSON_AddItemToArray(parsed_array, extra_str);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(parsed_array, false_item);

    // step 3: Operate
    cJSON_DeleteItemFromArray(parsed_array, 1);
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_elem", first_value);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}