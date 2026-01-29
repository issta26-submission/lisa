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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_false = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    const char *parse_end = (const char *)0;
    char json_buf[] = "{\"arr\": [ true, 123 ], \"keep\": \"yes\"}";
    char *out = (char *)0;
    cJSON_bool require_null = (cJSON_bool)1;
    cJSON_bool is_false = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_ParseWithOpts(json_buf, &parse_end, require_null);

    // step 3: Configure
    arr = cJSON_GetObjectItem(root, "arr");
    new_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, new_false);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 2);
    out = cJSON_PrintUnformatted(root);

    // step 5: Validate
    is_false = cJSON_IsFalse(got_item);
    cJSON_AddBoolToObject(root, "item_is_false", is_false);

    // step 6: Cleanup
    cJSON_free(out);
    cJSON_DeleteItemFromObject(root, "keep");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}