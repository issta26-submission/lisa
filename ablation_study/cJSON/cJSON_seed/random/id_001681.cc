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
//<ID> 1681
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *strArray = (cJSON *)0;
    cJSON *boolItem = (cJSON *)0;
    const char *strings[3] = { "alpha", "beta", "gamma" };
    double extracted_number = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    numItem = cJSON_CreateNumber(7.5);
    strItem = cJSON_CreateString("example");
    strArray = cJSON_CreateStringArray(strings, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", numItem);
    cJSON_AddItemToObject(root, "text", strItem);
    cJSON_AddItemToObject(root, "items", strArray);
    boolItem = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));

    // step 5: Validate
    validation = (int)(extracted_number == 7.5) + (int)(boolItem != (cJSON *)0) + (int)(cJSON_GetObjectItem(root, "items") != (cJSON *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)numItem;
    (void)strItem;
    (void)strArray;
    (void)boolItem;
    (void)extracted_number;

    // API sequence test completed successfully
    return 66;
}