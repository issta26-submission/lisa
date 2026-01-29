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
//<ID> 1685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *strArray = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    const char *strings[] = { "alpha", "beta", "gamma" };
    double num_value = 0.0;
    char *retrieved = (char *)0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    strArray = cJSON_CreateStringArray(strings, 3);
    strItem = cJSON_CreateString("greeting");
    numItem = cJSON_CreateNumber(123.456);

    // step 3: Configure
    cJSON_AddItemToObject(root, "strings", strArray);
    cJSON_AddItemToObject(root, "label", strItem);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "number", numItem);

    // step 4: Operate
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    retrieved = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));

    // step 5: Validate
    validation = (int)(num_value == 123.456) + (int)(retrieved != (char *)0) + (int)cJSON_HasObjectItem(root, "enabled");
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)strArray;
    (void)strItem;
    (void)numItem;
    (void)num_value;
    (void)retrieved;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}