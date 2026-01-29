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
//<ID> 1090
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
    cJSON_bool is_fetched_str_string = (cJSON_bool)0;
    double fetched_num_value = 0.0;
    const char *fetched_str_value = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi");

    // step 3: Configure - attach items to root
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    false_item = cJSON_AddFalseToObject(root, "valid");

    // step 4: Operate - fetch items and inspect
    fetched_num = cJSON_GetObjectItem(root, "number");
    fetched_str = cJSON_GetObjectItem(root, "name");
    fetched_false = cJSON_GetObjectItem(root, "valid");
    fetched_num_value = cJSON_GetNumberValue(fetched_num);
    fetched_str_value = cJSON_GetStringValue(fetched_str);
    is_fetched_str_string = cJSON_IsString(fetched_str);

    // step 5: Validate - serialize and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(num_item != (cJSON *)0)
                     + (int)(str_item != (cJSON *)0)
                     + (int)(false_item != (cJSON *)0)
                     + (int)(fetched_num != (cJSON *)0)
                     + (int)(fetched_str != (cJSON *)0)
                     + (int)(fetched_false != (cJSON *)0)
                     + (int)(is_fetched_str_string)
                     + (int)(fetched_num_value > 3.0)
                     + (int)(fetched_str_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root and attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}