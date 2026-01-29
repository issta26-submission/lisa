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
//<ID> 1823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    cJSON *added_number = (cJSON *)0;
    const char *strval = (const char *)0;
    double numval = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    added_number = cJSON_AddNumberToObject(root, "value", 3.14159);

    // step 4: Operate
    name_item = cJSON_GetObjectItem(root, "name");
    value_item = cJSON_GetObjectItem(root, "value");
    strval = cJSON_GetStringValue(name_item);
    numval = cJSON_GetNumberValue(value_item);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(value_item != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(added_number != (cJSON *)0) + (int)(numval == 3.14159);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)name_item;
    (void)value_item;
    (void)added_number;
    (void)strval;
    (void)numval;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}