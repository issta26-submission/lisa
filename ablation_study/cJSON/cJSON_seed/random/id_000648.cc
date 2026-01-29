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
//<ID> 648
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
    cJSON *extra_str = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_str0 = (cJSON *)0;
    cJSON *retrieved_true = (cJSON *)0;
    const char *names[2];
    int count = 2;
    cJSON_bool is_true = 0;
    const char *first_name = (const char *)0;
    int ptr_equal = 0;

    // step 2: Initialize
    names[0] = "Alice";
    names[1] = "Bob";
    strArray = cJSON_CreateStringArray((const char *const *)names, count);
    extra_str = cJSON_CreateString("Charlie");
    true_item = cJSON_CreateTrue();
    root = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "people", strArray);
    cJSON_AddItemToArray(strArray, extra_str);
    cJSON_AddItemToArray(strArray, true_item);

    // step 4: Operate
    retrieved_str0 = cJSON_GetArrayItem(strArray, 0);
    first_name = cJSON_GetStringValue(retrieved_str0);
    retrieved_true = cJSON_GetArrayItem(strArray, 3);
    is_true = cJSON_IsTrue(retrieved_true);

    // step 5: Validate
    ptr_equal = (retrieved_true == true_item);
    (void)first_name;
    (void)is_true;
    (void)ptr_equal;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}