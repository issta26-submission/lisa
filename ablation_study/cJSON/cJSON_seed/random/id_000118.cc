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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *name_copy = (cJSON *)0;
    cJSON *enabled_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool is_obj = 0;
    char *printed = (char *)0;
    const char *retrieved_name = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure - create items and add to root
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    enabled_item = cJSON_AddFalseToObject(root, "enabled");

    // step 4: Operate - inspect and duplicate data
    is_obj = cJSON_IsObject(root);
    printed = cJSON_Print(root);
    retrieved = cJSON_GetObjectItem(root, "name");
    retrieved_name = cJSON_GetStringValue(retrieved);
    name_copy = cJSON_CreateString(retrieved_name ? retrieved_name : "");
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 5: Validate - produce a fresh textual snapshot and free it
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}