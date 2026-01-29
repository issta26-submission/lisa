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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *added_sum = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *tmp = (cJSON *)0;
    int numbers[3] = {1, 2, 3};
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double extra = 0.0;
    double sum = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure
    int_array = cJSON_CreateIntArray(numbers, 3);
    add_ok_array = cJSON_AddItemToObject(root, "ints", int_array);
    str_item = cJSON_AddStringToObject(root, "note", "sample");
    num_item = cJSON_CreateNumber(10.5);
    add_ok_num = cJSON_AddItemToObject(root, "factor", num_item);

    // step 4: Operate
    tmp = cJSON_GetObjectItem(root, "ints");
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(tmp, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(tmp, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(tmp, 2));
    extra = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "factor"));
    sum = v0 + v1 + v2 + extra;
    added_sum = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", added_sum);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}