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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char *json_text = "{\"pi\":3.14159,\"msg\":\"hello\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *added_str = cJSON_CreateString("injected_value");
    cJSON *added_num = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "injected", added_str);
    cJSON_AddItemToObject(root, "answer", added_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *item_pi = cJSON_GetObjectItem(root, "pi");
    cJSON_bool pi_is_number = cJSON_IsNumber(item_pi);
    cJSON *item_answer = cJSON_GetObjectItem(root, "answer");
    cJSON_bool answer_is_number = cJSON_IsNumber(item_answer);
    (void)version;
    (void)pi_is_number;
    (void)answer_is_number;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}