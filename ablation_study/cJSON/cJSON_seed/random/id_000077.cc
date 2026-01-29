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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *values[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_array = cJSON_CreateStringArray(values, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_val = (const char *)0;

    // step 2: Initialize - attach created items to root
    cJSON_AddItemToObject(root, "items", str_array);
    num_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Configure - add a reference to the existing array in the object
    cJSON_AddItemReferenceToObject(root, "items_ref", str_array);

    // step 4: Operate - read back the first element from the array and re-add it as a named string
    retrieved_array = cJSON_GetObjectItem(root, "items");
    first_elem = cJSON_GetArrayItem(retrieved_array, 0);
    first_val = cJSON_GetStringValue(first_elem);
    cJSON_AddItemToObject(root, "first_item", cJSON_CreateString(first_val));

    // step 5: Validate - add a reference to the numeric item for inspection
    cJSON_AddItemReferenceToObject(root, "count_ref", num_item);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}