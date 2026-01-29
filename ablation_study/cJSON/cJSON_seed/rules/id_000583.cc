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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *true_ref = cJSON_CreateObjectReference(true_item);
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *retrieved0 = NULL;
    cJSON *retrieved1 = NULL;
    int arr_size = 0;
    cJSON_bool retrieved0_is_true = 0;
    cJSON_bool retrieved1_is_object = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemReferenceToArray(array, true_ref);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(array);
    retrieved0 = cJSON_GetArrayItem(array, 0);
    retrieved1 = cJSON_GetArrayItem(array, 1);
    retrieved0_is_true = cJSON_IsTrue(retrieved0);
    retrieved1_is_object = cJSON_IsObject(retrieved1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}