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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"numbers\":[1.5,2.5],\"Numbers\":[3.5]}";
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *arr_lower = (cJSON *)0;
    cJSON *arr_upper = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *item_case = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double vcase = 0.0;
    double total = 0.0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_ParseWithOpts(json, &parse_end, 1);
    new_num = cJSON_CreateNumber(4.5);

    // step 3: Configure
    arr_lower = cJSON_GetObjectItem(root, "numbers");
    arr_upper = cJSON_GetObjectItemCaseSensitive(root, "Numbers");
    add_ok = cJSON_AddItemToArray(arr_lower, new_num);

    // step 4: Operate
    item0 = cJSON_GetArrayItem(arr_lower, 0);
    item1 = cJSON_GetArrayItem(arr_lower, 1);
    item2 = cJSON_GetArrayItem(arr_lower, 2);
    item_case = cJSON_GetArrayItem(arr_upper, 0);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    v2 = cJSON_GetNumberValue(item2);
    vcase = cJSON_GetNumberValue(item_case);
    total = v0 + v1 + v2 + vcase;

    // step 5: Validate
    (void)add_ok;
    (void)parse_end;
    (void)total;
    (void)root;
    (void)arr_lower;
    (void)arr_upper;
    (void)item0;
    (void)item1;
    (void)item2;
    (void)item_case;
    (void)v0;
    (void)v1;
    (void)v2;
    (void)vcase;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}