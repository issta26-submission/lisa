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
//<ID> 1592
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, s3);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    int size = cJSON_GetArraySize(array);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "items_count", size_num);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *flag_false_num = cJSON_CreateNumber((double)flag_is_false);
    cJSON_AddItemToObject(root, "is_flag_false", flag_false_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}