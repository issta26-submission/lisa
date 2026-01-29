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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *fetched_value = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    char *printed = (char *)0;
    const char *label = (const char *)0;
    double value = 0.0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool add_ok_false = (cJSON_bool)0;
    cJSON_bool is_string = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi");

    // step 3: Configure - attach number, string and a false boolean to the root object
    add_ok_num = cJSON_AddItemToObject(root, "value", num_item);
    add_ok_str = cJSON_AddItemToObject(root, "label", str_item);
    false_item = cJSON_AddFalseToObject(root, "enabled");
    add_ok_false = (cJSON_bool)(false_item != (cJSON *)0);

    // step 4: Operate - fetch items and inspect types/values
    fetched_value = cJSON_GetObjectItem(root, "value");
    fetched_label = cJSON_GetObjectItem(root, "label");
    is_string = cJSON_IsString(fetched_label);
    value = cJSON_GetNumberValue(fetched_value);
    label = cJSON_GetStringValue(fetched_label);

    // step 5: Validate - serialize, compute a tiny validation score and free the buffer
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok_num) + (int)(add_ok_str) + (int)(add_ok_false) + (int)(is_string) + (int)(value > 3.0) + (int)(label != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}