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
//<ID> 1728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item1 = (cJSON *)0;
    cJSON *str_item2 = (cJSON *)0;
    char *extracted_str = (char *)0;
    int size_after_first_add = 0;
    int size_after_second_add = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item1 = cJSON_CreateString("hello");
    str_item2 = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", str_item1);
    size_after_first_add = cJSON_GetArraySize(root);
    cJSON_AddItemToObject(root, "noun", str_item2);
    size_after_second_add = cJSON_GetArraySize(root);

    // step 4: Operate
    extracted_str = cJSON_GetStringValue(str_item1);
    validation = (int)(extracted_str != (char *)0) + size_after_first_add + size_after_second_add;

    // step 5: Validate
    (void)validation;
    (void)extracted_str;
    (void)size_after_first_add;
    (void)size_after_second_add;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_item1;
    (void)str_item2;

    return 66;
    // API sequence test completed successfully
}