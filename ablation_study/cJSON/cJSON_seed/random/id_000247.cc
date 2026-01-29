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
//<ID> 247
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
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *manual_num = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    cJSON *fetched_answer = (cJSON *)0;
    char *printed = (char *)0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");

    // step 3: Configure
    num_item1 = cJSON_CreateNumber(10.0);
    num_item2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToArray(arr, num_item2);
    manual_num = cJSON_CreateNumber(42.0);
    add_ok = cJSON_AddItemToObject(root, "answer", manual_num);
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 4: Operate
    fetched_arr = cJSON_GetObjectItem(root, "numbers");
    fetched_item = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_answer = cJSON_GetObjectItem(root, "answer");
    v1 = cJSON_GetNumberValue(fetched_item);
    v2 = cJSON_GetNumberValue(fetched_answer);
    sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}