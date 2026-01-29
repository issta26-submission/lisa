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
//<ID> 1688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *names = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    const char *names_arr[3];
    double extracted_num = 0.0;
    int validation = 0;

    // step 2: Initialize
    names_arr[0] = "alpha";
    names_arr[1] = "beta";
    names_arr[2] = "gamma";
    root = cJSON_CreateObject();
    names = cJSON_CreateStringArray(names_arr, 3);
    num_item = cJSON_CreateNumber(123.456);
    label = cJSON_CreateString("example_label");

    // step 3: Configure
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "label", label);
    flag = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    extracted_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num"));

    // step 5: Validate
    validation = (int)(extracted_num == 123.456) + (int)(cJSON_GetObjectItem(root, "names") != (cJSON *)0) + (int)(cJSON_GetObjectItem(root, "enabled") != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)flag;
    (void)names;
    (void)num_item;
    (void)label;
    (void)extracted_num;

    // API sequence test completed successfully
    return 66;
}