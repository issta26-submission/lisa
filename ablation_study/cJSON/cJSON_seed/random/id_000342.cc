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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *wrapper = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem_from_ref = (cJSON *)0;
    cJSON *ref_second = (cJSON *)0;
    char *out = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateArray();
    wrapper = cJSON_CreateArray();

    // step 3: Configure
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(root, s1);
    cJSON_AddItemToArray(root, s2);
    ref = cJSON_CreateArrayReference(root);
    cJSON_AddItemToArray(wrapper, ref);

    // step 4: Operate
    elem0 = cJSON_GetArrayItem(root, 0);
    elem_from_ref = cJSON_GetArrayItem(wrapper, 0);
    ref_second = cJSON_GetArrayItem(elem_from_ref, 1);
    out = cJSON_PrintUnformatted(wrapper);

    // step 5: Validate
    (void)cJSON_GetArrayItem(root, 1);
    (void)cJSON_GetArrayItem(elem_from_ref, 0);
    (void)elem0;
    (void)ref_second;
    (void)out;

    // step 6: Cleanup
    if (out) { cJSON_free(out); }
    cJSON_Delete(wrapper);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}