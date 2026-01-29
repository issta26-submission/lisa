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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *subobj = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    char *val_orig = (char *)0;
    char *val_ref = (char *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    subobj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", subobj);

    // step 3: Configure
    str_item = cJSON_AddStringToObject(subobj, "greeting", "hello world");
    ref_item = cJSON_CreateObjectReference((const cJSON *)str_item);
    array = cJSON_CreateArray();
    add_ok = cJSON_AddItemToArray(array, ref_item);
    cJSON_AddItemToObject(root, "list", array);

    // step 4: Operate
    val_orig = (char *)cJSON_GetStringValue((const cJSON *const)str_item);
    val_ref = (char *)cJSON_GetStringValue((const cJSON *const)ref_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)val_orig;
    (void)val_ref;
    (void)add_ok;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}