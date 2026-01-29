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
//<ID> 708
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *pi_item = cJSON_AddNumberToObject(child, "pi", 3.14159);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(items, 0, s2);

    // step 2: Configure
    cJSON *found_pi = cJSON_GetObjectItemCaseSensitive(child, "pi");
    double pi_val = cJSON_GetNumberValue(found_pi);
    cJSON_AddNumberToObject(root, "twopi", pi_val * 2.0);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_str = cJSON_IsString(first_item);
    char *first_val = cJSON_GetStringValue(first_item);
    cJSON_AddNumberToObject(root, "first_is_string", (double)first_is_str);
    cJSON_AddStringToObject(root, "first_value", first_val);

    // step 3: Operate and Validate
    cJSON *copy = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(copy);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}