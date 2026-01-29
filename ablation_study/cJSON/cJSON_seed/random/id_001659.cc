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
//<ID> 1659
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
    cJSON *replacement = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_false = 0;
    int array_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    replacement = cJSON_CreateFalse();

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_Print(root);

    // step 5: Validate
    validation = (int)added0 + (int)added1 + (int)replaced + (int)is_false + (int)(array_size == 2);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)item0;
    (void)item1;
    (void)replacement;
    (void)added0;
    (void)added1;
    (void)replaced;
    (void)is_false;
    (void)array_size;
    // API sequence test completed successfully
    return 66;
}