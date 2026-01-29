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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", item_num);
    cJSON *item_str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", item_str);
    cJSON *item_bool = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", item_bool);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(item_str, "world");
    cJSON_AddStringToObject(root, "new_greet", set_ret);

    // step 3: Operate and Validate
    double num_val = cJSON_GetNumberValue(item_num);
    char *str_val = cJSON_GetStringValue(item_str);
    cJSON_bool is_false = cJSON_IsFalse(item_bool);
    cJSON_AddNumberToObject(root, "computed", num_val + (1 - is_false));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}