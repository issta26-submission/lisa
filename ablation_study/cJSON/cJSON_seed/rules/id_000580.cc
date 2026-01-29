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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *flag_name = "flag";
    cJSON *array = cJSON_CreateArray();
    cJSON *obj_child = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(obj_child, flag_name, true_item);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj_child);
    cJSON_AddItemToArray(array, obj_ref);

    // step 3: Operate / Validate
    int array_size = cJSON_GetArraySize(array);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    cJSON *retrieved_flag = cJSON_GetObjectItem(first_item, flag_name);
    cJSON_bool flag_is_true = cJSON_IsTrue(retrieved_flag);
    int consumed_size = array_size + (flag_is_true ? 1 : 0);
    (void)consumed_size;
    (void)scratch;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(array);
    cJSON_Delete(obj_child);

    // API sequence test completed successfully
    return 66;
}