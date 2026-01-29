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
//<ID> 1773
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
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    char *strval = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("hello");
    s1 = cJSON_CreateString("world");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "greetings", arr);

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "greetings");
    first_item = cJSON_GetArrayItem(retrieved_items, 0);
    strval = cJSON_GetStringValue(first_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(strval != (char *)0) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)retrieved_items;
    (void)first_item;
    (void)strval;
    (void)add_ok1;
    (void)add_ok2;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}