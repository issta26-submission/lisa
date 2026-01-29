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
//<ID> 654
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
    cJSON *item2 = (cJSON *)0;
    cJSON *retr0 = (cJSON *)0;
    cJSON *retr1 = (cJSON *)0;
    cJSON *retr2 = (cJSON *)0;
    cJSON_bool added = 0;
    int array_size = 0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    char *val2 = (char *)0;
    int first_ch0 = 0;
    int first_ch1 = 0;
    int first_ch2 = 0;
    int pointers_equal0 = 0;
    int pointers_equal1 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("red");
    item1 = cJSON_CreateString("green");
    item2 = cJSON_CreateString("blue");

    // step 3: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    added = cJSON_AddItemToObject(root, "colors", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retr0 = cJSON_GetArrayItem(arr, 0);
    retr1 = cJSON_GetArrayItem(arr, 1);
    retr2 = cJSON_GetArrayItem(arr, 2);
    val0 = cJSON_GetStringValue(retr0);
    val1 = cJSON_GetStringValue(retr1);
    val2 = cJSON_GetStringValue(retr2);
    first_ch0 = val0 ? (int)val0[0] : 0;
    first_ch1 = val1 ? (int)val1[0] : 0;
    first_ch2 = val2 ? (int)val2[0] : 0;

    // step 5: Validate
    pointers_equal0 = (val0 == item0->valuestring);
    pointers_equal1 = (val1 == item1->valuestring);
    (void)added;
    (void)array_size;
    (void)retr0;
    (void)retr1;
    (void)retr2;
    (void)val0;
    (void)val1;
    (void)val2;
    (void)first_ch0;
    (void)first_ch1;
    (void)first_ch2;
    (void)pointers_equal0;
    (void)pointers_equal1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}