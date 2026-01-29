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
//<ID> 1683
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    const char *words[3];
    double num_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    words[0] = "alpha";
    words[1] = "beta";
    words[2] = "gamma";
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray((const char *const *)words, 3);
    str_item = cJSON_CreateString("single");
    num_item = cJSON_CreateNumber(13.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "list", str_array);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "value", num_item);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    num_val = cJSON_GetNumberValue(num_item);

    // step 5: Validate
    validation = (int)(num_val == 13.5) + (int)(bool_item != (cJSON *)0) + (int)(cJSON_GetNumberValue(num_item) == 13.5);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_array;
    (void)str_item;
    (void)num_item;
    (void)bool_item;
    (void)num_val;

    // API sequence test completed successfully
    return 66;
}