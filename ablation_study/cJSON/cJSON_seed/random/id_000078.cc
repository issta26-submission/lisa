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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *strings_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    const char *items[] = { "first", "second", "third" };

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(items, 3);

    // step 3: Configure - attach created items to root, add numeric and boolean entries, and a reference
    cJSON_AddItemToObject(root, "items", str_array);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemReferenceToObject(root, "items_ref", str_array);

    // step 4: Operate - read back values and compute a derived value, then store results
    count_item = cJSON_GetObjectItem(root, "count");
    cJSON_AddNumberToObject(root, "count_double", cJSON_GetNumberValue(count_item) * 2.0);
    strings_item = cJSON_GetObjectItem(root, "items");
    first_elem = cJSON_GetArrayItem(strings_item, 0);
    first_str = cJSON_GetStringValue(first_elem);
    cJSON_AddItemToObject(root, "first_item_copy", cJSON_CreateString(first_str));

    // step 5: Validate - add a boolean validation marker based on existing true item (propagate the same)
    cJSON_AddItemToObject(root, "validated", cJSON_CreateTrue());

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}