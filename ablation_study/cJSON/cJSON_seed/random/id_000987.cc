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
//<ID> 987
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *str_key = "greeting";
    const char *num_key = "pi";
    const char *str_val = "hello cJSON";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool add_str_ok = (cJSON_bool)0;
    char *json_str = (char *)0;
    cJSON *fetched_str = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    const char *retrieved_cstr = (const char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object and a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(str_val);

    // step 3: Configure - attach the string using AddItemToObject and add a number using AddNumberToObject
    add_str_ok = cJSON_AddItemToObject(root, str_key, str_item);
    num_item = cJSON_AddNumberToObject(root, num_key, 3.141592653589793);

    // step 4: Operate - serialize the root to a string
    json_str = cJSON_Print(root);

    // step 5: Validate - retrieve items using both GetObjectItem and CaseSensitive variant and extract values
    fetched_str = cJSON_GetObjectItem(root, str_key);
    fetched_num = cJSON_GetObjectItemCaseSensitive(root, num_key);
    retrieved_cstr = cJSON_GetStringValue(fetched_str);
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    validation_score = (int)(root != (cJSON *)0) + (int)(add_str_ok) + (int)(num_item != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(retrieved_cstr != (const char *)0 && retrieved_cstr[0] == 'h') + (int)(retrieved_num > 3.0 && retrieved_num < 4.0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}