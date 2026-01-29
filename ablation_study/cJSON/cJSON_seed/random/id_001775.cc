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
//<ID> 1775
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_element = (cJSON *)0;
    char *printed = (char *)0;
    const char *str_value = (const char *)0;
    int validation = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    add_ok3 = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_items = cJSON_GetObjectItem(root, "items");
    retrieved_element = cJSON_GetArrayItem(retrieved_items, 1);
    str_value = cJSON_GetStringValue(retrieved_element);

    // step 5: Validate
    validation = (int)(printed != 0) + (int)(retrieved_items != 0) + (int)(retrieved_element != 0) + (int)(str_value != 0) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)retrieved_items;
    (void)retrieved_element;
    (void)str_value;
    (void)printed;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}