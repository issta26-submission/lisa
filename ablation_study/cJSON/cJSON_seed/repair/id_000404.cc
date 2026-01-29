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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "copied", dup_items);

    // step 3: Operate
    cJSON *orig_items_ref = cJSON_GetObjectItem(root, "items");
    int orig_size = cJSON_GetArraySize(orig_items_ref);
    cJSON *copied_ref = cJSON_GetObjectItem(root, "copied");
    int copied_size = cJSON_GetArraySize(copied_ref);
    cJSON *first_in_copy = cJSON_GetArrayItem(copied_ref, 0);
    char *updated = cJSON_SetValuestring(first_in_copy, "alpha_updated");
    (void)updated;
    double num_val = cJSON_GetNumberValue(n1);
    double combined = (double)orig_size + (double)copied_size + num_val;
    cJSON_AddNumberToObject(root, "computed", combined);

    // step 4: Validate and Cleanup
    int final_orig_size = cJSON_GetArraySize(items);
    int final_copied_size = cJSON_GetArraySize(copied_ref);
    (void)final_orig_size;
    (void)final_copied_size;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}