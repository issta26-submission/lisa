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
//<ID> 459
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
    cJSON *subobj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *ref_subobj = (cJSON *)0;
    char *printed = (char *)0;
    const char *sval = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    subobj = cJSON_CreateObject();
    str_item = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddStringToObject(subobj, "greeting", "hello");
    cJSON_AddItemToArray(arr, str_item);
    ref_subobj = cJSON_CreateObjectReference((const cJSON *)subobj);
    cJSON_AddItemToArray(arr, ref_subobj);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", subobj);

    // step 4: Operate
    sval = cJSON_GetStringValue((const cJSON *const)str_item);
    cJSON_AddStringToObject(root, "first", sval);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)printed;
    (void)sval;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}