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
//<ID> 650
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
    cJSON *retr0 = (cJSON *)0;
    cJSON *retr1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    int array_size = 0;
    char *str0 = (char *)0;
    char *str1 = (char *)0;
    int nonnull_count = 0;
    int pointers_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("hello");
    item1 = cJSON_CreateString("world");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added_obj = cJSON_AddItemToObject(root, "greetings", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retr0 = cJSON_GetArrayItem(arr, 0);
    retr1 = cJSON_GetArrayItem(arr, 1);
    str0 = cJSON_GetStringValue(retr0);
    str1 = cJSON_GetStringValue(retr1);

    // step 5: Validate
    nonnull_count = (str0 != (char *)0) + (str1 != (char *)0);
    pointers_equal = (str0 == item0->valuestring) + (str1 == item1->valuestring);
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)array_size;
    (void)nonnull_count;
    (void)pointers_equal;
    (void)retr0;
    (void)retr1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}