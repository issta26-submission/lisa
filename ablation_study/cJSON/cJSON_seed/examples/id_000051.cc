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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.718);
    cJSON *num3 = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_bool added1 = cJSON_AddItemToArray(array, num1);
    cJSON_bool added2 = cJSON_AddItemToArray(array, num2);
    cJSON_bool added3 = cJSON_AddItemToArray(array, num3);
    cJSON_bool added_to_object = cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate & Validate
    char *printed_before = cJSON_PrintUnformatted(root);
    double status_val = (double)added1 + (double)added2 + (double)added3 + (double)added_to_object;
    cJSON *status_num = cJSON_CreateNumber(status_val);
    cJSON_AddItemToObject(root, "status", status_num);
    char *printed_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed_before);
    cJSON_free(printed_after);

    // API sequence test completed successfully
    return 66;
}