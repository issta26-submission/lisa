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
//<ID> 1820
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
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_number = (cJSON *)0;
    const char *strval = (const char *)0;
    double numval = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddNumberToObject(root, "value", 3.1415);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    strval = cJSON_GetStringValue(retrieved_name);
    retrieved_number = cJSON_GetObjectItem(root, "value");
    numval = cJSON_GetNumberValue(retrieved_number);

    // step 5: Validate
    validation = (int)(retrieved_name != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(retrieved_number != (cJSON *)0) + (int)(numval != 0.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)name_item;
    (void)retrieved_name;
    (void)retrieved_number;
    (void)strval;
    (void)numval;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}