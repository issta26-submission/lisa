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
//<ID> 60
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
    cJSON *greet_item = cJSON_CreateString("hello world");
    cJSON *arr_member = cJSON_CreateString("member1");
    char *greet_value = (char *)0;

    // step 2: Setup - attach array and greeting to root
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "greeting", greet_item);

    // step 3: Configure - add a reference to the same array under a different name
    cJSON_AddItemReferenceToObject(root, "array_ref", arr);

    // step 4: Operate - populate the array
    cJSON_AddItemToArray(arr, arr_member);

    // step 5: Validate / manipulate - read the greeting string and create a copy, then remove the original
    greet_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    cJSON_AddItemToObject(root, "greeting_copy", cJSON_CreateString(greet_value));
    cJSON_DeleteItemFromObject(root, "greeting");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}