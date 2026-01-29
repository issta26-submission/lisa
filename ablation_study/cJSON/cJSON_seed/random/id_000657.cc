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
//<ID> 657
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
    cJSON *str_item0 = (cJSON *)0;
    cJSON *str_item1 = (cJSON *)0;
    cJSON *retr0 = (cJSON *)0;
    cJSON *retr1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    int array_size = 0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    int score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item0 = cJSON_CreateString("orange");
    str_item1 = cJSON_CreateString("grape");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str_item0);
    added1 = cJSON_AddItemToArray(arr, str_item1);
    cJSON_AddItemToObject(root, "fruits", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retr0 = cJSON_GetArrayItem(arr, 0);
    retr1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetStringValue(retr0);
    val1 = cJSON_GetStringValue(retr1);
    score = (int)val0[0] + (int)val1[0] + array_size;

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)retr0;
    (void)retr1;
    (void)val0;
    (void)val1;
    (void)score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}