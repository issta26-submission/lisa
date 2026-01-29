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
//<ID> 668
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed_root = (char *)0;
    char *printed_detached = (char *)0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    int nonnull_count = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_obj = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");

    // step 3: Configure
    added_arr = cJSON_AddItemToArray(arr, item0);
    (void)added_arr;
    added_arr = cJSON_AddItemToArray(arr, item1);
    (void)added_arr;
    added_obj = cJSON_AddItemToObject(root, "items", arr);
    (void)added_obj;

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "extracted", detached);
    val0 = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetStringValue(detached);
    printed_root = cJSON_PrintUnformatted(root);
    printed_detached = cJSON_PrintUnformatted(detached);

    // step 5: Validate
    nonnull_count = (val0 != (char *)0) + (val1 != (char *)0) + (printed_root != (char *)0) + (printed_detached != (char *)0);
    (void)nonnull_count;
    (void)val0;
    (void)val1;

    // step 6: Cleanup
    if (printed_root) { cJSON_free(printed_root); }
    if (printed_detached) { cJSON_free(printed_detached); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}