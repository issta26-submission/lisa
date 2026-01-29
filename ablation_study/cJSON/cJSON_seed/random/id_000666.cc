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
//<ID> 666
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *val0 = (char *)0;
    char *valDetached = (char *)0;
    char *printed = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool addedObj = 0;
    cJSON_bool addedDetached = 0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("hello");
    str1 = cJSON_CreateString("world");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str0);
    added1 = cJSON_AddItemToArray(arr, str1);
    addedObj = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    val0 = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    valDetached = cJSON_GetStringValue(detached);
    printed = cJSON_PrintUnformatted(root);
    addedDetached = cJSON_AddItemToObject(root, "detached", detached);

    // step 5: Validate
    nonnull_count = (val0 != (char *)0) + (valDetached != (char *)0) + (printed != (char *)0);
    (void)added0;
    (void)added1;
    (void)addedObj;
    (void)addedDetached;
    (void)nonnull_count;
    (void)val0;
    (void)valDetached;
    (void)detached;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}