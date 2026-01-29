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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *list_obj = (cJSON *)0;
    cJSON *list_first = (cJSON *)0;
    cJSON *num_from_a = (cJSON *)0;
    cJSON *num_from_list = (cJSON *)0;
    const char *json = "{\"a\":1.5,\"b\":2.25,\"list\":[3.75,4.5]}";
    const char *endptr = (const char *)0;
    double a_val = 0.0;
    double list_first_val = 0.0;
    char *printed = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;

    // step 2: Initialize
    parsed = cJSON_ParseWithOpts(json, &endptr, 1);
    arr = cJSON_CreateArray();

    // step 3: Configure
    item_a = cJSON_GetObjectItemCaseSensitive(parsed, "a");
    a_val = cJSON_GetNumberValue(item_a);
    num_from_a = cJSON_CreateNumber(a_val);
    added1 = cJSON_AddItemToArray(arr, num_from_a);
    list_obj = cJSON_GetObjectItem(parsed, "list");
    list_first = cJSON_GetArrayItem(list_obj, 0);
    list_first_val = cJSON_GetNumberValue(list_first);
    num_from_list = cJSON_CreateNumber(list_first_val);
    added2 = cJSON_AddItemToArray(arr, num_from_list);
    cJSON_AddItemToObject(parsed, "merged", arr);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(parsed);

    // step 5: Validate
    (void)endptr;
    (void)added1;
    (void)added2;
    (void)printed;
    (void)a_val;
    (void)list_first_val;

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}