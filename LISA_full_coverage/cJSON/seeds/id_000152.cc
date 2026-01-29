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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *array_ref = cJSON_CreateArray();
    cJSON_AddItemToArray(array_ref, cJSON_CreateBool(1));
    cJSON_AddItemToObject(root, "flag_array", array_ref);

    // step 2: Configure
    const int PREBUF_LEN = 256;
    char *prebuf = (char *)cJSON_malloc((size_t)PREBUF_LEN);
    memset(prebuf, 0, PREBUF_LEN);
    cJSON_bool printed_preallocated = cJSON_PrintPreallocated(root, prebuf, PREBUF_LEN, 1);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(array_ref);
    cJSON *first_elem = cJSON_GetArrayItem(array_ref, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_elem);
    char *unformatted = cJSON_PrintUnformatted(root);
    int summary = arr_size + (int)first_is_bool + (int)printed_preallocated + (unformatted ? (int)unformatted[0] : 0) + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}