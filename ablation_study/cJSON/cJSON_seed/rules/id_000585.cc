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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_text = "fuzz_object_child";
    cJSON *array = cJSON_CreateArray();
    cJSON *array2 = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *obj_ref = cJSON_CreateObjectReference(true_item);
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    int size_array = 0;
    int size_array2 = 0;
    cJSON *first_in_array = NULL;
    cJSON *first_in_array2 = NULL;
    int total_count = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array2, obj_ref);
    cJSON_AddItemReferenceToArray(array, obj_ref);

    // step 3: Operate / Validate
    size_array = cJSON_GetArraySize(array);
    size_array2 = cJSON_GetArraySize(array2);
    first_in_array = cJSON_GetArrayItem(array, 0);
    first_in_array2 = cJSON_GetArrayItem(array2, 0);
    total_count = size_array + size_array2;
    scratch[0] = (char)('0' + (total_count % 10));
    scratch[1] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(array2);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}