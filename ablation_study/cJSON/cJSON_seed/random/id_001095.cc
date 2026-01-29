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
//<ID> 1095
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
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    cJSON *fetched_false = (cJSON *)0;
    char *printed = (char *)0;
    double num_value = 0.0;
    const char *str_value = (const char *)0;
    cJSON_bool str_is_string = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi_label");

    // step 3: Configure - attach the number and string to the root object and add a false flag
    add_ok_num = cJSON_AddItemToObject(root, "value", num_item);
    add_ok_str = cJSON_AddItemToObject(root, "label", str_item);
    false_item = cJSON_AddFalseToObject(root, "enabled");

    // step 4: Operate - fetch the items and read their values
    fetched_num = cJSON_GetObjectItem(root, "value");
    fetched_str = cJSON_GetObjectItem(root, "label");
    fetched_false = cJSON_GetObjectItem(root, "enabled");
    num_value = cJSON_GetNumberValue(fetched_num);
    str_value = cJSON_GetStringValue(fetched_str);
    str_is_string = cJSON_IsString(fetched_str);

    // step 5: Validate - serialize and compute a small score from results
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(fetched_false != (cJSON *)0) + (int)(add_ok_num) + (int)(add_ok_str) + (int)(str_is_string) + (int)(num_value == 3.1415) + (int)(str_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}