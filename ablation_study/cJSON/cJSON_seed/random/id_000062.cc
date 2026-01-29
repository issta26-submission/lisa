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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON *copied = (cJSON *)0;
    const cJSON *ref_item = (cJSON *)0;
    const char *val = (const char *)0;

    // step 2: Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);

    // step 3: Operate
    ref_item = cJSON_GetObjectItem(root, "shared_ref");
    val = cJSON_GetStringValue(ref_item);
    copied = cJSON_CreateString(val);
    cJSON_AddItemToObject(root, "copied", copied);

    // step 4: Validate
    val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "copied"));
    (void)val;

    // step 5: Cleanup
    cJSON_DeleteItemFromObject(root, "shared_ref");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}