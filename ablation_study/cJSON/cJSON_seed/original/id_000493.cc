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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"data\":{\"numbers\":[1,2,3],\"meta\":{}},\"info\":\"sample\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *numbers = cJSON_GetObjectItem(data, "numbers");
    cJSON_bool numbers_is_array = cJSON_IsArray(numbers);
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 2: Configure
    cJSON *added_false = cJSON_AddFalseToObject(data, "enabled");
    cJSON *added_null = cJSON_AddNullToObject(data, "optional");
    cJSON *meta = cJSON_GetObjectItem(data, "meta");
    cJSON *copied_value = cJSON_Duplicate(first, 0);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(new_obj, "copied", copied_value);
    cJSON_AddItemToObject(meta, "nested", new_obj);
    cJSON *computed_num = cJSON_CreateNumber(first_val + (double)numbers_is_array);
    cJSON_AddItemToObject(root, "computed", computed_num);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}