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
//<ID> 63
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *item0 = (cJSON *)0;
    cJSON *copied = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    char *val0 = (char *)0;
    char *ref_val = (char *)0;
    cJSON *mirror = (cJSON *)0;

    // step 2: Setup - attach array and items
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate - read first item's string, create a copy and add to root, add reference
    item0 = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetStringValue(item0);
    copied = cJSON_CreateString(val0);
    cJSON_AddItemToObject(root, "copied_item", copied);
    cJSON_AddItemReferenceToObject(root, "ref_item", item0);

    // step 4: Validate - retrieve referenced string value and mirror it into root
    ref = cJSON_GetObjectItem(root, "ref_item");
    ref_val = cJSON_GetStringValue(ref);
    mirror = cJSON_CreateString(ref_val);
    cJSON_AddItemToObject(root, "mirror", mirror);

    // step 5: Configure/Modify - delete the copied item from root
    cJSON_DeleteItemFromObject(root, "copied_item");

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}