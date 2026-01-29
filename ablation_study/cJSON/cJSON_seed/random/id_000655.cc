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
//<ID> 655
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
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    int array_size = 0;
    char *str0 = (char *)0;
    char *str1 = (char *)0;
    int first_letters_check = 0;

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
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    str0 = cJSON_GetStringValue(got0);
    str1 = cJSON_GetStringValue(got1);

    // step 5: Validate
    first_letters_check = (str0[0] == 'h') + (str1[0] == 'w');
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)array_size;
    (void)got0;
    (void)got1;
    (void)str0;
    (void)str1;
    (void)first_letters_check;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}