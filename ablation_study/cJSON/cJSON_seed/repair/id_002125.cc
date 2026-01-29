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
//<ID> 2125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_bool added_to_array = cJSON_AddItemToArray(arr, s1);
    cJSON *owner_str = cJSON_AddStringToObject(meta, "owner", "tester");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddNumberToObject(root, "added_flag", (double)added_to_array);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItemCaseSensitive(root, "answer");
    double val = cJSON_GetNumberValue(got);
    cJSON_AddNumberToObject(root, "copied_answer", val);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}