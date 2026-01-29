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
//<ID> 1178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *name_item = NULL;
    cJSON *copied_name = NULL;
    cJSON *str_array = NULL;
    cJSON *number_item = NULL;
    cJSON *root = NULL;
    cJSON *retrieved_name = NULL;
    const char *retrieved_str = NULL;
    const char *name_value = NULL;
    const char *arr[] = { "alpha", "beta", "gamma" };
    char json[] = "{\"name\":\"Bob\",\"list\":[\"a\",\"b\",\"c\"]}";
    char *printed = NULL;
    double set_ret = 0.0;
    double num_val = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    name_item = cJSON_GetObjectItem(parsed, "name");
    name_value = cJSON_GetStringValue(name_item);
    copied_name = cJSON_CreateString(name_value);
    str_array = cJSON_CreateStringArray(arr, 3);
    cJSON_DeleteItemFromArray(str_array, 1);
    number_item = cJSON_CreateNumber(0.0);
    set_ret = cJSON_SetNumberHelper(number_item, 7.25);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name_copy", copied_name);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON_AddItemToObject(root, "value", number_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    num_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    retrieved_name = cJSON_GetObjectItem(root, "name_copy");
    retrieved_str = cJSON_GetStringValue(retrieved_name);
    (void)retrieved_str;
    (void)num_val;
    (void)set_ret;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}