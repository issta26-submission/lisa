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
//<ID> 400
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
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    char *json_str = (char *)0;
    const char *extracted = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);
    got_item = cJSON_GetArrayItem(arr, 0);

    // step 4: Validate
    extracted = cJSON_GetStringValue(got_item);
    (void)extracted;
    (void)bool_item;
    (void)got_item;

    // step 5: Modify
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");

    // step 6: Cleanup
    cJSON_free((void *)json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}