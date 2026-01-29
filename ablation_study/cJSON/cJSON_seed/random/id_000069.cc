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
//<ID> 69
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
    cJSON *temp = cJSON_CreateString("transient");
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_val = (const char *)0;

    // step 2: Setup - populate array
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Configure - attach array and a transient item to root and add a reference to the array
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "temp", temp);
    cJSON_AddItemReferenceToObject(root, "items_ref", arr);

    // step 4: Operate - retrieve the first element's string value from the array
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetStringValue(first_item);

    // step 5: Validate - store the retrieved string back into the root as a named item
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateString(first_val));

    // step 6: Cleanup
    cJSON_DeleteItemFromObject(root, "temp");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}