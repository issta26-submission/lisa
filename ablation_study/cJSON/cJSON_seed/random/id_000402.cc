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
//<ID> 402
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
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *extracted = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("hello-world");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddStringToObject(root, "name", "to-be-deleted");

    // step 4: Operate
    first_item = cJSON_GetArrayItem(arr, 0);
    printed = cJSON_Print((const cJSON *)root);
    cJSON_Minify(printed);

    // step 5: Validate
    extracted = cJSON_GetStringValue(first_item);
    (void)extracted;
    (void)first_item;

    // step 6: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_Delete(root);
    cJSON_free((void *)printed);

    // API sequence test completed successfully
    return 66;
}