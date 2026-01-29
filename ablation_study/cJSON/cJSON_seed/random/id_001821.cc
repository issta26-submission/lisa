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
//<ID> 1821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_child = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    const char *strval = (const char *)0;
    double retrieved_num = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_child = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", str_child);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate
    str_item = cJSON_GetObjectItem(root, "greeting");
    strval = cJSON_GetStringValue(str_item);
    num_item = cJSON_GetObjectItem(root, "answer");
    retrieved_num = cJSON_GetNumberValue(num_item);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(str_child != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(num_item != (cJSON *)0) + (int)(retrieved_num == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_child;
    (void)str_item;
    (void)num_item;
    (void)strval;
    (void)retrieved_num;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}