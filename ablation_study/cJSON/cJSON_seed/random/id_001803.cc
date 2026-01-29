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
//<ID> 1803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *initial_str = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *replacement_false = (cJSON *)0;
    cJSON *restored_str = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool replaced1 = 0;
    cJSON_bool replaced2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    initial_str = cJSON_CreateString("initial_value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "field", initial_str);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "field");
    replacement_false = cJSON_CreateFalse();
    replaced1 = cJSON_ReplaceItemViaPointer(root, retrieved, replacement_false);
    retrieved = cJSON_GetObjectItem(root, "field");
    restored_str = cJSON_CreateString("restored_value");
    replaced2 = cJSON_ReplaceItemViaPointer(root, retrieved, restored_str);
    retrieved = cJSON_GetObjectItem(root, "field");
    strval = cJSON_GetStringValue(retrieved);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(replacement_false != (cJSON *)0) + (int)(restored_str != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(replaced1 != 0) + (int)(replaced2 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)initial_str;
    (void)retrieved;
    (void)replacement_false;
    (void)restored_str;
    (void)strval;
    (void)replaced1;
    (void)replaced2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}