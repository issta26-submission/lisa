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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14159);
    cJSON *n2 = cJSON_CreateNumber(2.71828);
    cJSON *n3 = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_bool added_n1 = cJSON_AddItemToArray(numbers, n1);
    cJSON_bool added_n2 = cJSON_AddItemToArray(numbers, n2);
    cJSON_bool added_array = cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_SetNumberHelper(n3, 42.0);
    cJSON_bool added_n3 = cJSON_AddItemToObject(root, "answer", n3);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    cJSON *flag_added_n1 = cJSON_CreateNumber((double)added_n1);
    cJSON *flag_added_n2 = cJSON_CreateNumber((double)added_n2);
    cJSON *flag_added_array = cJSON_CreateNumber((double)added_array);
    cJSON_AddItemToObject(root, "added_n1", flag_added_n1);
    cJSON_AddItemToObject(root, "added_n2", flag_added_n2);
    cJSON_AddItemToObject(root, "added_array", flag_added_array);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}