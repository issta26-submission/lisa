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
//<ID> 64
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
    cJSON *str_item2 = cJSON_CreateString("extra");
    cJSON *first_item = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    char *extracted = (char *)0;

    // step 2: Initialize - populate array with string items
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, str_item2);

    // step 3: Configure - attach array to root and add a reference to the first element
    cJSON_AddItemToObject(root, "items", arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_AddItemReferenceToObject(root, "first", first_item);

    // step 4: Operate - retrieve the referenced item and extract its string value
    ref_item = cJSON_GetObjectItem(root, "first");
    extracted = cJSON_GetStringValue(ref_item);
    cJSON_AddStringToObject(root, "copied", extracted);

    // step 5: Validate - remove the copied standalone string from root
    cJSON_DeleteItemFromObject(root, "copied");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}