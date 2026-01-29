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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "[10,20,30]";
    cJSON *array = cJSON_Parse(json);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_InsertItemInArray(array, 1, false_item);

    // step 3: Operate
    cJSON *inserted = cJSON_GetArrayItem(array, 1);
    cJSON_bool is_false = cJSON_IsFalse(inserted);
    char *printed = cJSON_Print(array);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    (void)is_false;
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}