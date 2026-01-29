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
//<ID> 457
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
    cJSON *child_obj = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_title = (cJSON *)0;
    char *title_str = (char *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first_entry");
    s1 = cJSON_CreateString("second_entry");
    child_obj = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddStringToObject(child_obj, "title", "child_title");         // required API
    cJSON_AddItemToArray(arr, s0);                                     // required API
    cJSON_AddItemToArray(arr, s1);                                     // required API (again, legitimate use)
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "child", child_obj);
    child_ref = cJSON_CreateObjectReference((const cJSON *)child_obj);  // required API
    cJSON_AddItemToArray(arr, child_ref);                              // add reference into array

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    got_title = cJSON_GetObjectItem(got_child, "title");
    title_str = (char *)cJSON_GetStringValue((const cJSON *const)got_title); // required API
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)title_str;
    (void)printed;
    (void)got_child;
    (void)got_title;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}