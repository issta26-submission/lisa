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
//<ID> 1808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    cJSON *retrieved_second = (cJSON *)0;
    const char *first_str = (const char *)0;
    cJSON_bool replace_ok = 0;
    cJSON_bool second_is_false = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    first = cJSON_CreateString("original");
    second = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "first", first);
    cJSON_AddItemToObject(root, "second", second);
    replacement = cJSON_CreateString("replacement");

    // step 4: Operate
    retrieved_first = cJSON_GetObjectItem(root, "first");
    replace_ok = cJSON_ReplaceItemViaPointer(root, retrieved_first, replacement);
    retrieved_first = cJSON_GetObjectItem(root, "first");
    retrieved_second = cJSON_GetObjectItem(root, "second");
    first_str = cJSON_GetStringValue(retrieved_first);
    second_is_false = cJSON_IsFalse(retrieved_second);

    // step 5: Validate
    validation = (int)(replace_ok != 0) + (int)(first_str != (const char *)0) + (int)(second_is_false != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)first;
    (void)second;
    (void)replacement;
    (void)retrieved_first;
    (void)retrieved_second;
    (void)first_str;
    (void)replace_ok;
    (void)second_is_false;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}