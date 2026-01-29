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
//<ID> 769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved);
    (void)cJSON_IsObject(root); /* use as part of validation below */
    (void)cJSON_IsObject(meta);

    // step 5: Validate
    validation_score = (retrieved != (cJSON *)0) + (retrieved_str != (char *)0) + (int)added0 + (int)added1 + (int)cJSON_IsObject(root) + (int)cJSON_IsObject(meta);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}