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
//<ID> 1597
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
    cJSON_AddItemToObject(root, "letters", array);

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "is_valid", flag_false);
    int count = cJSON_GetArraySize(array);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 3: Operate & Validate
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    cJSON *is_false_num = cJSON_CreateNumber((double)is_false);
    cJSON_AddItemToObject(root, "is_false_flag", is_false_num);
    cJSON *note = cJSON_CreateString("array_initialized_and_validated");
    cJSON_AddItemToObject(root, "note", note);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}