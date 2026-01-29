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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"title\":\"root\"}");
    cJSON *aux = cJSON_Parse("{\"num\":3}");
    cJSON *bfalse = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON_AddItemToArray(flags, bfalse);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_GetObjectItem(aux, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    int flags_size = cJSON_GetArraySize(flags);
    cJSON *first_in_flags = cJSON_GetArrayItem(flags, 0);
    int validation_sum = (int)cJSON_IsFalse(first_in_flags) + (flags_size == 1) + (int)num_val;
    cJSON_AddNumberToObject(root, "validation", (double)validation_sum);

    // step 4: Cleanup
    cJSON_Delete(aux);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}