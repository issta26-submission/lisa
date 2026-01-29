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
//<ID> 1166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"value\":3.5}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();

    // step 2: Configure
    cJSON *one = cJSON_CreateNumber(1.0);
    cJSON *two = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(numbers, one);
    cJSON_AddItemToArray(numbers, two);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddNumberToObject(root, "base", 10.0);

    // step 3: Operate & Validate
    cJSON_bool has_flag = cJSON_HasObjectItem(parsed, "flag");
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    double base_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "base"));
    double computed = base_val + (double)has_flag + 2.0 * (double)flag_is_false;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}