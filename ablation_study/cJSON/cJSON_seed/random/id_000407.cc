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
//<ID> 407
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
    cJSON *got = (cJSON *)0;
    char *printed = (char *)0;
    const char *extracted = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    got = cJSON_GetArrayItem(arr, 0);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_Minify(printed);

    // step 5: Validate
    extracted = cJSON_GetStringValue(got);
    (void)extracted;
    (void)got;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}