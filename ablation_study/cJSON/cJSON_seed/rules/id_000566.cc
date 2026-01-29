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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    char *unformatted = NULL;
    cJSON *retrieved = NULL;
    int arr_size = 0;
    int idx = 0;
    cJSON_bool is_str = 0;
    const char *val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, item1);
    cJSON_AddItemReferenceToArray(array, item2);

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted(array);
    arr_size = cJSON_GetArraySize(array);
    idx = arr_size - 1;
    retrieved = cJSON_GetArrayItem(array, idx);
    is_str = cJSON_IsString(retrieved);
    val = cJSON_GetStringValue(retrieved);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}