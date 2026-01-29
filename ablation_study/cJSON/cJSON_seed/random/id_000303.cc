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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *parsed_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    char json_buf[] = "[ false, true, null ]";
    cJSON_bool require_terminated = (cJSON_bool)1;

    // step 2: Initialize
    parsed = cJSON_ParseWithOpts(json_buf, &parse_end, require_terminated);
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    parsed_item = cJSON_GetArrayItem(parsed, 0);
    dup_item = cJSON_Duplicate(parsed_item, (cJSON_bool)1);
    cJSON_AddItemToArray(array, dup_item);
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddItemToObject(root, "manual_false", false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "manual_false");

    // step 5: Validate
    cJSON_AddNumberToObject(root, "data_count", (double)cJSON_GetArraySize(array));
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}