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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *sub = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *strnode = (cJSON *)0;
    cJSON *obj_ref = (cJSON *)0;
    char *strval = (char *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    sub = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    s1 = cJSON_AddStringToObject(sub, "label", "hello");
    strnode = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, strnode);

    // add a reference to 'sub' into the array using the proper API to avoid double-free
    cJSON_AddItemReferenceToArray(arr, sub);
    // obtain the reference item for inspection if needed
    obj_ref = cJSON_GetArrayItem(arr, 1);

    cJSON_AddItemToObject(root, "meta", sub);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    strval = cJSON_GetStringValue(s1);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate (avoid unused warning)
    (void)strval;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}