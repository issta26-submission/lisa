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
//<ID> 1807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *oldstr = (cJSON *)0;
    cJSON *newstr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_before = (cJSON *)0;
    cJSON *retrieved_after = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    oldstr = cJSON_CreateString("original_value");
    newstr = cJSON_CreateString("replacement_value");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "field", oldstr);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    retrieved_before = cJSON_GetObjectItem(root, "field");
    replace_ok = cJSON_ReplaceItemViaPointer(root, retrieved_before, newstr);
    retrieved_after = cJSON_GetObjectItem(root, "field");
    strval = cJSON_GetStringValue(retrieved_after);

    // step 5: Validate
    validation = (int)(retrieved_before != (cJSON *)0) + (int)(retrieved_after != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(replace_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)oldstr;
    (void)newstr;
    (void)false_item;
    (void)retrieved_before;
    (void)retrieved_after;
    (void)strval;
    (void)replace_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}