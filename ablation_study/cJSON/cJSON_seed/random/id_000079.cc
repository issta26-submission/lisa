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
//<ID> 79
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_str = (const char *)0;
    const char *items[] = { "one", "two", "three" };

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray(items, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemReferenceToObject(root, "items_ref", arr);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemToObject(root, "ok", cJSON_CreateTrue());

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_item);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateString(first_str));

    // step 5: Validate
    cJSON_AddNumberToObject(root, "first_length", (double) (first_str ? (int)strlen(first_str) : 0));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}