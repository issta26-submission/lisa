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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"num\": 42, \"text\": \"hello\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddFalseToObject(root, "disabled");
    cJSON_AddItemToObject(root, "source", parsed_dup);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *num_item = cJSON_CreateNumber(123.5);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate and Validate
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Minify(json_text);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(root_copy);

    // API sequence test completed successfully
    return 66;
}