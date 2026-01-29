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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    (void)version;
    const char *json_text = "{\"num\": 42.5, \"greeting\": \"hello\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *added_str = cJSON_CreateString("added_value");

    // step 2: Configure
    cJSON_AddItemToObject(root, "extra", added_str);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool is_num = cJSON_IsNumber(num_item);
    (void)is_num;
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(greet_item);
    (void)greet_val;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}