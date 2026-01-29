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
//<ID> 1680
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *strNode = (cJSON *)0;
    cJSON *strArray = (cJSON *)0;
    cJSON *numNode = (cJSON *)0;
    const char *items[3];
    double retrieved_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    strNode = cJSON_CreateString("example_label");
    items[0] = "alpha";
    items[1] = "beta";
    items[2] = "gamma";
    strArray = cJSON_CreateStringArray(items, 3);
    numNode = cJSON_CreateNumber(7.25);

    // step 3: Configure
    cJSON_AddItemToObject(root, "label", strNode);
    cJSON_AddItemToObject(root, "list", strArray);
    cJSON_AddItemToObject(root, "value", numNode);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    retrieved_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));

    // step 5: Validate
    validation = (int)(retrieved_value == 7.25) + (int)cJSON_HasObjectItem(root, "list") + (int)cJSON_IsString(cJSON_GetObjectItem(root, "label"));
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)strNode;
    (void)strArray;
    (void)numNode;
    (void)retrieved_value;

    // API sequence test completed successfully
    return 66;
}