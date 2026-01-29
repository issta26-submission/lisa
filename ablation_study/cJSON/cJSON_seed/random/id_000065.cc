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
//<ID> 65
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
    cJSON *dup_str = (cJSON *)0;
    char *val = (char *)0;

    // step 2: Setup - attach array to root and populate array with a string item
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate - read the string value from the array item and create a duplicate
    val = cJSON_GetStringValue(str_item);
    dup_str = cJSON_CreateString(val);
    cJSON_AddItemToObject(root, "copied", dup_str);

    // step 4: Configure - add a reference to the original string item into root
    cJSON_AddItemReferenceToObject(root, "ref", str_item);

    // step 5: Validate/Cleanup - remove the copied and the reference, then delete root
    cJSON_DeleteItemFromObject(root, "copied");
    cJSON_DeleteItemFromObject(root, "ref");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}