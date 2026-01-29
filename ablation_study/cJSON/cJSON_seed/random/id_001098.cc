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
//<ID> 1098
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
    cJSON_bool is_string = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi_label");
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "label", str_item);
    false_item = cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate - fetch items and inspect types/values
    fetched_num = cJSON_GetObjectItem(root, "value");
    fetched_str = cJSON_GetObjectItem(root, "label");
    fetched_false = cJSON_GetObjectItem(root, "flag");
    num_value = cJSON_GetNumberValue(fetched_num);
    str_value = cJSON_GetStringValue(fetched_str);
    is_string = cJSON_IsString(fetched_str);

    // step 4: Validate - serialize and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(fetched_false != (cJSON *)0) + (int)(is_string) + (int)(num_value == 3.1415) + (int)(str_value != (const char *)0);
    (void)validation_score;

    // step 5: Cleanup - free serialized buffer and delete root (frees children)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}