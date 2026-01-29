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
//<ID> 1719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_lower = (cJSON *)0;
    cJSON *num_upper = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_lower = (cJSON *)0;
    cJSON *retrieved_upper = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double value_lower = 0.0;
    double value_upper = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_lower = cJSON_CreateNumber(10.0);
    num_upper = cJSON_CreateNumber(20.0);
    label = cJSON_CreateString("ten and twenty");

    // step 3: Configure
    cJSON_AddItemToObject(root, "value", num_lower);
    cJSON_AddItemToObject(root, "Value", num_upper);
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    retrieved_lower = cJSON_GetObjectItem(root, "value");
    retrieved_upper = cJSON_GetObjectItemCaseSensitive(root, "Value");
    value_lower = cJSON_GetNumberValue(retrieved_lower);
    value_upper = cJSON_GetNumberValue(retrieved_upper);

    // step 5: Validate
    validation = (int)(value_lower == 10.0) + (int)(value_upper == 20.0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_lower;
    (void)num_upper;
    (void)label;
    (void)retrieved_lower;
    (void)retrieved_upper;
    (void)value_lower;
    (void)value_upper;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}