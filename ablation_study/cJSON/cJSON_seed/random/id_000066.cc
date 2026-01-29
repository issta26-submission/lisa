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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *alias_item = (cJSON *)0;
    const char *retrieved = (const char *)0;

    // step 2: Setup - populate array and attach to root
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 3: Reference - add a reference to the string item into root
    cJSON_AddItemReferenceToObject(root, "alias", str_item);

    // step 4: Operate/Validate - read the referenced string and duplicate it into a new named item
    alias_item = cJSON_GetObjectItem(root, "alias");
    retrieved = cJSON_GetStringValue(alias_item);
    cJSON_AddItemToObject(root, "copied", cJSON_CreateString(retrieved));

    // step 5: Cleanup - remove the alias reference and delete the whole root
    cJSON_DeleteItemFromObject(root, "alias");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}