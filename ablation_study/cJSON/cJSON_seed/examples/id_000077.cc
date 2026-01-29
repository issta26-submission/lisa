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
    // step 1: Initialize
    char json[] = " { \"name\" : \"Alice\", \"age\" : 30, \"info\": { \"note\": \" sample \" } } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    cJSON *age_plus_two = cJSON_CreateNumber(age_val + 2.0);
    cJSON_AddItemToObject(root, "age_plus_two", age_plus_two);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *greeting_prefix = cJSON_CreateString("Hello");
    cJSON_AddItemToObject(root, "greeting_prefix", greeting_prefix);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *equal_flag = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "dup_equal", equal_flag);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}