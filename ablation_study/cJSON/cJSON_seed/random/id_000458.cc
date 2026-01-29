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
//<ID> 458
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
    cJSON *s_primary = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    char *val = (char *)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure
    s_primary = cJSON_AddStringToObject(root, "primary", "alpha");
    ref = cJSON_CreateObjectReference((const cJSON *)s_primary);
    cJSON_AddItemToArray(arr, ref);
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    val = cJSON_GetStringValue((const cJSON *)got0);
    cJSON_AddStringToObject(root, "copied", val);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)val;
    (void)got0;
    (void)s_primary;
    (void)s2;
    (void)ref;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}