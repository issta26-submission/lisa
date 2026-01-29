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
//<ID> 308
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
    cJSON *got_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    const char *parse_end = (const char *)0;
    char *printed = (char *)0;
    char json_buf[] = "{\"arr\":[1,2],\"keep\":\"yes\"}";
    cJSON_bool require_null_terminated = (cJSON_bool)1;

    // step 2: Initialize
    root = cJSON_ParseWithOpts(json_buf, &parse_end, require_null_terminated);

    // step 3: Configure
    arr = cJSON_GetObjectItem(root, "arr");
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 2);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "keep");

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}