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
//<ID> 656
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
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_to_obj = 0;
    int array_size = 0;
    cJSON *retr0 = (cJSON *)0;
    cJSON *retr1 = (cJSON *)0;
    cJSON *retr2 = (cJSON *)0;
    char *val0 = (char *)0;
    char *val1 = (char *)0;
    char *val2 = (char *)0;
    int pointers_match = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    s1 = cJSON_CreateString("second");
    s2 = cJSON_CreateString("third");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, s0);
    added1 = cJSON_AddItemToArray(arr, s1);
    added2 = cJSON_AddItemToArray(arr, s2);
    added_to_obj = cJSON_AddItemToObject(root, "sequence", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retr0 = cJSON_GetArrayItem(arr, 0);
    retr1 = cJSON_GetArrayItem(arr, 1);
    retr2 = cJSON_GetArrayItem(arr, 2);
    val0 = cJSON_GetStringValue(retr0);
    val1 = cJSON_GetStringValue(retr1);
    val2 = cJSON_GetStringValue(retr2);

    // step 5: Validate
    pointers_match = (val0 == s0->valuestring) + (val1 == s1->valuestring) + (val2 == s2->valuestring);
    (void)added0;
    (void)added1;
    (void)added2;
    (void)added_to_obj;
    (void)array_size;
    (void)retr0;
    (void)retr1;
    (void)retr2;
    (void)val0;
    (void)val1;
    (void)val2;
    (void)pointers_match;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}