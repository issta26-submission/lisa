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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(100.5);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *greet_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "flag", flag_item);
    cJSON_AddItemToObject(root, "greeting", greet_item);

    // step 2: Configure
    double base_val = cJSON_GetNumberValue(num_item);
    cJSON_bool flag_val = cJSON_IsTrue(flag_item);
    char *greet_str = cJSON_GetStringValue(greet_item);
    double first_char_val = (greet_str && greet_str[0]) ? (double)(unsigned char)greet_str[0] : 0.0;
    cJSON *derived_item = cJSON_CreateNumber(base_val + (double)flag_val + first_char_val);
    cJSON_AddItemToObject(root, "derived", derived_item);

    // step 3: Operate and Validate
    cJSON *got_derived = cJSON_GetObjectItem(root, "derived");
    double derived_val = cJSON_GetNumberValue(got_derived);
    cJSON *half_item = cJSON_CreateNumber(derived_val * 0.5);
    cJSON_AddItemToObject(root, "half", half_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}