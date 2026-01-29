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
//<ID> 1712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *found_num = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(7.5);
    str_item = cJSON_CreateString("seven");

    // step 3: Configure
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemToObject(child, "label", str_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    found_child = cJSON_GetObjectItem(root, "child");
    found_num = cJSON_GetObjectItemCaseSensitive(found_child, "num");
    extracted_value = cJSON_GetNumberValue(found_num);

    // step 5: Validate
    validation = (int)(extracted_value == 7.5) + (int)(json_unformatted != (char *)0) + (int)(found_child != (cJSON *)0) + (int)(found_num != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)child;
    (void)num_item;
    (void)str_item;
    (void)found_child;
    (void)found_num;
    (void)extracted_value;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}