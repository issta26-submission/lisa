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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "flag", false_item);
    cJSON_AddNumberToObject(child, "answer", 42.0);

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 0, child);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    cJSON *retrieved_arr = cJSON_GetObjectItem(root, "items");
    cJSON *first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    cJSON *flag_obj = cJSON_GetObjectItem(first_elem, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_obj);
    cJSON_AddBoolToObject(root, "flag_false_copy", flag_is_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}