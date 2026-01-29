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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *nested_obj = NULL;
    cJSON *item_name = NULL;
    cJSON *item_value = NULL;
    cJSON *item_inner = NULL;
    char *name_str = NULL;
    double value_num = 0.0;
    double inner_num = 0.0;
    double sum = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 123.5);
    cJSON_AddNumberToObject(nested, "inner", 7.25);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate / Validate
    item_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(item_name);
    item_value = cJSON_GetObjectItem(root, "value");
    value_num = cJSON_GetNumberValue(item_value);
    nested_obj = cJSON_GetObjectItem(root, "nested");
    item_inner = cJSON_GetObjectItem(nested_obj, "inner");
    inner_num = cJSON_GetNumberValue(item_inner);
    sum = value_num + inner_num;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}