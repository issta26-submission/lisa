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
//<ID> 1772
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    char *printed = (char *)0;
    char *strval = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s0);
    add_ok2 = cJSON_AddItemToArray(arr, s1);
    add_ok3 = cJSON_AddItemToObject(root, "names", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "names");
    arr_item = cJSON_GetArrayItem(retrieved_arr, 1);
    strval = cJSON_GetStringValue(arr_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(add_ok3 != 0) + (int)(printed != (char *)0) + (int)(strval && strval[0] == 't');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)s0;
    (void)s1;
    (void)retrieved_arr;
    (void)arr_item;
    (void)strval;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}