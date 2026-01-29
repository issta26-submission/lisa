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
//<ID> 984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_msg = "msg";
    const char *key_value = "value";
    const char *str_payload = "payload";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_msg = (cJSON *)0;
    cJSON *fetched_value_cs = (cJSON *)0;
    char *json_str = (char *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_num = 0.0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(str_payload);

    // step 3: Configure - attach the string using AddItemToObject and add a number using AddNumberToObject
    add_ok = cJSON_AddItemToObject(root, key_msg, str_item);
    num_item = cJSON_AddNumberToObject(root, key_value, 123.456);

    // step 4: Operate - serialize the root and retrieve items via generic and case-sensitive getters
    json_str = cJSON_PrintUnformatted(root);
    fetched_msg = cJSON_GetObjectItem(root, key_msg);
    fetched_value_cs = cJSON_GetObjectItemCaseSensitive(root, key_value);
    retrieved_str = cJSON_GetStringValue(fetched_msg);
    retrieved_num = cJSON_GetNumberValue(fetched_value_cs);

    // step 5: Validate - compute a simple validation score that exercises results
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok) + (int)(num_item != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 'p') + (int)(retrieved_num == 123.456);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}