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
//<ID> 586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"a\": 1.5, \"list\": [2.5]}";
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *new_n1 = (cJSON *)0;
    cJSON *new_n2 = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *retr_list = (cJSON *)0;
    double value_a = 0.0;
    double value_third = 0.0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;

    // step 2: Initialize / Parse
    root = cJSON_ParseWithOpts(json, &parse_end, 1);
    arr = cJSON_GetObjectItem(root, "list");

    // step 3: Configure - create numbers and add them to the parsed array
    new_n1 = cJSON_CreateNumber(3.75);
    new_n2 = cJSON_CreateNumber(4.25);
    added1 = cJSON_AddItemToArray(arr, new_n1);
    added2 = cJSON_AddItemToArray(arr, new_n2);

    // step 4: Operate - retrieve values via case-sensitive and normal getters
    item_a = cJSON_GetObjectItemCaseSensitive(root, "a");
    value_a = cJSON_GetNumberValue(item_a);
    retr_list = cJSON_GetObjectItem(root, "list");
    value_third = cJSON_GetNumberValue(cJSON_GetArrayItem(retr_list, 2));

    // step 5: Validate (use values to satisfy data flow, avoid unused warnings)
    (void)parse_end;
    (void)added1;
    (void)added2;
    (void)value_a;
    (void)value_third;
    (void)item_a;
    (void)new_n1;
    (void)new_n2;
    (void)arr;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}